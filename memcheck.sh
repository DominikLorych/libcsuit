#!/bin/bash

TESTFILE_PATH="${TESTFILE_PATH:=testfiles/}"
LIBCSUIT_PATH="${LIBCSUIT_EXEC:=./bin/}"

exec="suit_manifest_process"
echo "Running valgrind massif profiling on ${exec}..."
for f in ${TESTFILE_PATH}/*.cbor
do
  echo "    File: $f..."
  valgrind -q --tool=massif --heap=yes --stacks=yes --massif-out-file=massif.out ${LIBCSUIT_PATH}${exec} $f >/dev/null
  mem_heap_b=$(cat massif.out | grep "mem_heap_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
  mem_heap_extra_b=$(cat massif.out | grep "mem_heap_extra_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
  mem_stacks_b=$(cat massif.out | grep "mem_stacks_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
  total=$((mem_heap_b+mem_heap_extra_b+mem_stacks_b))
  echo "    Memory Usage (B): Total | Heap | Heap Extra | Stack (see valgrind massif for more details)"
  echo "    ${total} | ${mem_heap_b} | ${mem_heap_extra_b} | ${mem_stacks_b}"
  rm massif.out
done

exec="suit_manifest_parser"
echo "Running valgrind massif profiling on ${exec}..."
for f in ${TESTFILE_PATH}/*.cbor
do
  echo "    File: $f..."
  valgrind -q --tool=massif --heap=yes --stacks=yes --massif-out-file=massif.out ${LIBCSUIT_PATH}${exec} $f >/dev/null
  mem_heap_b=$(cat massif.out | grep "mem_heap_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
  mem_heap_extra_b=$(cat massif.out | grep "mem_heap_extra_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
  mem_stacks_b=$(cat massif.out | grep "mem_stacks_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
  total=$((mem_heap_b+mem_heap_extra_b+mem_stacks_b))
  echo "    Memory Usage (B): Total | Heap | Heap Extra | Stack (see valgrind massif for more details)"
  echo "    ${total} | ${mem_heap_b} | ${mem_heap_extra_b} | ${mem_stacks_b}"
  rm massif.out
done

exec="suit_manifest_encode"
echo "Running valgrind massif profiling on ${exec}..."
valgrind -q --tool=massif --heap=yes --stacks=yes --massif-out-file=massif.out ${LIBCSUIT_PATH}${exec} >/dev/null
mem_heap_b=$(cat massif.out | grep "mem_heap_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
mem_heap_extra_b=$(cat massif.out | grep "mem_heap_extra_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
mem_stacks_b=$(cat massif.out | grep "mem_stacks_B" | grep -G -o "[0-9]*" | sort -n | tail -1)
total=$((mem_heap_b+mem_heap_extra_b+mem_stacks_b))
echo "    Memory Usage (B): Total | Heap | Heap Extra | Stack (see valgrind massif for more details)"
echo "    ${total} | ${mem_heap_b} | ${mem_heap_extra_b} | ${mem_stacks_b}"
rm massif.out