#pragma once

template<typename element_t>
void array_copy(element_t *source, size_t n, element_t *dest) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = source[i];
    }
}
