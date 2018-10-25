# compile la bibliothèque
make -C hp_allocator/hp_allocator_malloc_free/SRC/ shared

# compile le benchmark et execution avec la bibliothèque
make -C svalat_bench_lib/
LD_LIBRARY_PATH=hp_allocator/hp_allocator_malloc_free/SRC/:$LD_LIBRARY_PATH ./svalat_bench_lib/bench_hp_malloc
