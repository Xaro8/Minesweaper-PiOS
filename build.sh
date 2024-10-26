~/llvm-project/rbuild/bin/clang -target pcpu-unknown-pios-elf --sysroot ../../compile_base -I ../../lib/libpios/ -v main.c -o test
mv test ../../base/bin/
cd ..
./pack_sd.sh
cd /home/kiril/pcsn

cargo run --release -- ~/piOS/kernel/build/kernel_text.bin ~/piOS/kernel/build/kernel_data.bin ~/piOS/test.tar > /dev/null