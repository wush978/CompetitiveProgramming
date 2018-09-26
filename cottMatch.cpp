#include <cmath>
#include <iostream>
#include <vector>
#include <set>

#ifdef USING_R
#include <Rcpp.h>
#endif

struct CompByRef {
  const double* ref;
  const int len;
  CompByRef(const double* _ref, const int _len) :
    ref(_ref), len(_len)
  { }
  bool operator()(int i, int j) {
    if (i < 0 || j < 0) return i < j;
    if (i >= len || j >= len) return i < j;
    return (ref[i] == ref[j] ? i < j : ref[i] < ref[j]);
  };
};

int get_cott(
  const double* df_long,
  const double* df_lat,
  const double* df_time,
  const int df_size,
  const double* cott_long,
  const double* cott_lat,
  const double* cott_time,
  const int cott_size,
  int* retval,
  double sp_threshold = 0.001,
  double time_threshold = 86400 * 7
) {
  const double shift = 0.0005;
  const double sp_threshold_sq = std::pow(sp_threshold, 2);
  const double *long_start = cott_long, *long_end;
  const double *cott_long_end = cott_long + cott_size;

  CompByRef c_lat(cott_lat, cott_size), c_time(cott_time, cott_size);
  std::set< int , CompByRef >
    cott_lat_idx((c_lat)),
    cott_time_idx((c_time));

  std::set< int , CompByRef >::const_iterator lat_start, lat_end, time_start, time_end, k;

  while(long_start != cott_long_end && *long_start < df_long[0] + shift - sp_threshold) {
    long_start++;
  }
  long_end = long_start;
  while(long_end != cott_long_end && *long_end <= df_long[0] + shift + sp_threshold) {
    cott_lat_idx.insert(long_end - cott_long);
    long_end++;
  }

  bool
    update_long(false),
    reset_lat(true),
    update_lat(false),
    reset_time(true),
    update_time(false);

  for(int i = 0;i < df_size;i++) {
    if (i > 0) {
      update_long = df_long[i] != df_long[i-1];
      reset_lat = update_long;
      update_lat = (!reset_lat) && (df_lat[i] != df_lat[i-1]);
      reset_time = reset_lat || update_lat;
      update_time = !reset_time;
    }

    if (update_long) {
      while(long_start != cott_long_end && *long_start < df_long[i] + shift - sp_threshold) {
        cott_lat_idx.erase(long_start - cott_long);
        long_start++;
      }
      while(long_end != cott_long_end && *long_end <= df_long[i] + shift + sp_threshold) {
        if (long_end >= long_start) cott_lat_idx.insert(long_end - cott_long);
        long_end++;
      }
    }

    if (long_end - long_start != cott_lat_idx.size()) {
      throw std::logic_error("invalid cott_lat_idx size");
    }

    if (reset_lat) {
      cott_time_idx.clear();
      lat_start = cott_lat_idx.begin();
      while(lat_start != cott_lat_idx.end() && cott_lat[*lat_start] < df_lat[i] + shift - sp_threshold) {
        lat_start++;
      }
      lat_end = lat_start;
      while(lat_end != cott_lat_idx.end() && cott_lat[*lat_end] <= df_lat[i] + shift + sp_threshold) {
        cott_time_idx.insert(*lat_end);
        lat_end++;
      }
    }

    if (update_lat) {
      bool is_empty = lat_start == lat_end;
      while(lat_start != cott_lat_idx.end() && cott_lat[*lat_start] < df_lat[i] + shift - sp_threshold) {
        if (!is_empty) cott_time_idx.erase(*lat_start);
        lat_start++;
        if (lat_start == lat_end) is_empty = true;
      }
      while(lat_end != cott_lat_idx.end() && cott_lat[*lat_end] <= df_lat[i] + shift + sp_threshold) {
        if (lat_start == lat_end) is_empty = false;
        if (!is_empty) cott_time_idx.insert(*lat_end);
        lat_end++;
      }
    }

    {
      std::set< int , CompByRef >::const_iterator it = lat_start;
      for(int i = 0;i < cott_time_idx.size();i++) {
        it++;
      }
      if (it != lat_end) {
        throw std::logic_error("invalid cott_time_idx size");
      }
    }

    if (reset_time) {
      time_start = cott_time_idx.begin();
      while(time_start != cott_time_idx.end() && cott_time[*time_start] <= df_time[i]) {
        time_start++;
      }
      time_end = time_start;
      while(time_end != cott_time_idx.end() && cott_time[*time_end] <= df_time[i] + time_threshold) {
        time_end++;
      }
    }

    if (update_time) {
      while(time_start != cott_time_idx.end() && cott_time[*time_start] <= df_time[i]) {
        time_start++;
      }
      while(time_end != cott_time_idx.end() && cott_time[*time_end] <= df_time[i] + time_threshold) {
        time_end++;
      }
    }

    retval[i] = 0;
    for(k = time_start;k != time_end;k++) {
      double sp_dist = std::pow(df_long[i] + shift - cott_long[*k], 2);
      sp_dist += std::pow(df_lat[i] + shift - cott_lat[*k], 2);
      if (sp_dist < sp_threshold_sq) {
        retval[i]++;
      }
    }
  }

  return 0;
}

#ifdef USING_R

using namespace Rcpp;

//[[Rcpp::export("get.cott")]]
IntegerVector get_cott(
  NumericVector df_long,
  NumericVector df_lat,
  NumericVector df_time,
  NumericVector cott_long,
  NumericVector cott_lat,
  NumericVector cott_time,
  double sp_threshold,
  double time_threshold
) {
  IntegerVector retval(df_long.size(), 0);
  get_cott(
    df_long.begin(),
    df_lat.begin(),
    df_time.begin(),
    df_long.size(),
    cott_long.begin(),
    cott_lat.begin(),
    cott_time.begin(),
    cott_long.size(),
    retval.begin(),
    sp_threshold,
    time_threshold
  );
  return retval;
}

#else

int main() {
  int df_size, cott_size;
  if (!(std::cin >> df_size)) return 1;
  if (!(std::cin >> cott_size)) return 2;
  std::vector<double>
    df_long(df_size, 0.0), df_lat(df_size, 0.0), df_time(df_size, 0.0),
    cott_long(cott_size, 0.0), cott_lat(cott_size, 0.0), cott_time(cott_size, 0.0);
  for(int i = 0;i < df_size;i++) {
    if (!(std::cin >> df_long[i] >> df_lat[i] >> df_time[i])) return 3;
  }
  for(int i = 0;i < cott_size;i++) {
    if (!(std::cin >> cott_long[i] >> cott_lat[i] >> cott_time[i])) return 4;
  }
  std::vector<int> retval(df_size, 0);
  if (get_cott(
    df_long.data(), df_lat.data(), df_time.data(), df_size,
    cott_long.data(), cott_lat.data(), cott_time.data(), cott_size,
    retval.data()
  )) return 5;
  for(int i = 0;i < df_size;i++) {
    std::cout << retval[i] << " ";
  }
  std::cout << std::endl;
  return 0;
}

#endif
