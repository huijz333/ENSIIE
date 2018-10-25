# compile la bibliothèque
make -C hp_allocator/hp_allocator_malloc_free/SRC/ shared

# compile le benchmark et execution avec la bibliothèque
make -C svalat_bench
LD_PRELOAD=hp_allocator/hp_allocator_malloc_free/SRC/libhp_allocator.so ./svalat_bench/bench_malloc
LD_PRELOAD=hp_allocator/hp_allocator_malloc_free/SRC/libhp_allocator.so ./svalat_bench/bench_huge
