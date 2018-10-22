import lldb
import lldb.formatters.Logger

class stddeque_SynthProvider:

    def __init__(self, valobj, d):
        print "test"
        logger = lldb.formatters.Logger.Logger()
        logger.write("init")
        self.valobj = valobj
        self.pointer_size = self.valobj.GetProcess().GetAddressByteSize()
        self.count = None
        try:
            self.find_block_size()
        except:
            self.block_size = -1
            self.element_size = -1
        logger.write(
            "block_size=%d, element_size=%d" %
            (self.block_size, self.element_size))

    def find_block_size(self):
        # in order to use the deque we must have the block size, or else
        # it's impossible to know what memory addresses are valid
        self.element_type = self.valobj.GetType().GetTemplateArgumentType(0)
        self.element_size = self.element_type.GetByteSize()
        # The code says this, but there must be a better way:
        # template <class _Tp, class _Allocator>
        # class __deque_base {
        #    static const difference_type __block_size = sizeof(value_type) < 256 ? 4096 / sizeof(value_type) : 16;
        # }
        if self.element_size < 256:
            self.block_size = 4096 / self.element_size
        else:
            self.block_size = 16

    def num_children(self):
        global _deque_capping_size
        logger = lldb.formatters.Logger.Logger()
        if self.count is None:
            return 0
        return min(self.count, _deque_capping_size)

    def has_children(self):
        return True

    def get_child_index(self, name):
        logger = lldb.formatters.Logger.Logger()
        try:
            return int(name.lstrip('[').rstrip(']'))
        except:
            return -1

    def get_child_at_index(self, index):
        logger = lldb.formatters.Logger.Logger()
        logger.write("Fetching child " + str(index))
        if index < 0 or self.count is None:
            return None
        if index >= self.num_children():
            return None
        try:
            i, j = divmod(self.start + index, self.block_size)
            return self.first.CreateValueFromExpression(
                '[' + str(index) + ']', '*(*(%s + %d) + %d)' %
                (self.first.get_expr_path(), i, j))
        except:
            return None

    def _get_value_of_compressed_pair(self, pair):
        value = pair.GetChildMemberWithName("__value_")
        if not value.IsValid():
            # pre-r300140 member name
            value = pair.GetChildMemberWithName("__first_")
        return value.GetValueAsUnsigned(0)

    def update(self):
        logger = lldb.formatters.Logger.Logger()
        try:
            # A deque is effectively a two-dim array, with fixed width.
            # 'map' contains pointers to the rows of this array. The
            # full memory area allocated by the deque is delimited
            # by 'first' and 'end_cap'. However, only a subset of this
            # memory contains valid data since a deque may have some slack
            # at the front and back in order to have O(1) insertion at
            # both ends. The rows in active use are delimited by
            # 'begin' and 'end'.
            #
            # To find the elements that are actually constructed, the 'start'
            # variable tells which element in this NxM array is the 0th
            # one, and the 'size' element gives the number of elements
            # in the deque.
            count = self._get_value_of_compressed_pair(
                    self.valobj.GetChildMemberWithName('__size_'))
            # give up now if we cant access memory reliably
            if self.block_size < 0:
                logger.write("block_size < 0")
                return
            map_ = self.valobj.GetChildMemberWithName('__map_')
            start = self.valobj.GetChildMemberWithName(
                '__start_').GetValueAsUnsigned(0)
            first = map_.GetChildMemberWithName('__first_')
            map_first = first.GetValueAsUnsigned(0)
            map_begin = map_.GetChildMemberWithName(
                '__begin_').GetValueAsUnsigned(0)
            map_end = map_.GetChildMemberWithName(
                '__end_').GetValueAsUnsigned(0)
            map_endcap = self._get_value_of_compressed_pair(
                    map_.GetChildMemberWithName( '__end_cap_'))
            # check consistency
            if not map_first <= map_begin <= map_end <= map_endcap:
                logger.write("map pointers are not monotonic")
                return
            total_rows, junk = divmod(
                map_endcap - map_first, self.pointer_size)
            if junk:
                logger.write("endcap-first doesnt align correctly")
                return
            active_rows, junk = divmod(map_end - map_begin, self.pointer_size)
            if junk:
                logger.write("end-begin doesnt align correctly")
                return
            start_row, junk = divmod(map_begin - map_first, self.pointer_size)
            if junk:
                logger.write("begin-first doesnt align correctly")
                return
            if not start_row * \
                    self.block_size <= start < (start_row + 1) * self.block_size:
                logger.write("0th element must be in the 'begin' row")
                return
            end_row = start_row + active_rows
            if not count:
                if active_rows:
                    logger.write("empty deque but begin!=end")
                    return
            elif not (end_row - 1) * self.block_size <= start + count < end_row * self.block_size:
                logger.write("nth element must be before the 'end' row")
                return
            logger.write(
                "update success: count=%r, start=%r, first=%r" %
                (count, start, first))
            # if consistent, save all we really need:
            self.count = count
            self.start = start
            self.first = first
        except:
            self.count = None
            self.start = None
            self.map_first = None
            self.map_begin = None

_deque_capping_size = 255