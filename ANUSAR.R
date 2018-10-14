#!/usr/bin/Rscript
f <- function(n) {
	x <- sample(letters, n, replace = TRUE)
	x <- paste(x, collapse = "")
	c(1, x, format(n, scientific = FALSE), 1:n)
}

for(i in 2:5) {
	x <- f(2 * 10^i)
	cat(x, file = sprintf("ANUSAR-%d.input", i))
}
