for f in Makefile include/*.h src/*.c utils/*.c shaders/*.vert shaders/*.frag; do
  if [ -f "$f" ]; then
    echo "=== FILE: $f ==="
    cat "$f"
    echo -e "\n"
  fi
done > /tmp/vulkanC.txt
