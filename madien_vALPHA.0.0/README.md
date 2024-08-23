# MaDiEn source

## Notable behavior

This section is dedicated to some behavioral details one might want to know.

###  eb_init()

`eb_init()` returns the type `int`, with `0` corresponding to success. Other return codes are:  
- `1`: Failure to allocate memory for `data` member.
- `2`: The buffer set to be initialized has a set `data` member already. This will only appear if `MDE_BUFFER_ALLOC_CHECK` is set.

### eb_set()

`eb_set()` returns the type `int`, with `0` corresponding to success. Other return codes are:  
- `1`: The specified x and y coordinates were outside the buffer's range. This will only appear if `MDE_BUFFER_INDEX_CHECK` is set.

### eb_str_put()

If `eb_str_put()`'s `strlen` argument is `0`, it is set to `buf->width - offs_x` to fill the entire line.
