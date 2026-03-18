#!/usr/bin/env bash
set -euo pipefail

echo "🧹 Cleaning old build artifacts..."

OLD_OBJECTS=$(find . -name "*.o" 2>/dev/null || true)
if [ -n "$OLD_OBJECTS" ]; then
  while IFS= read -r file; do
    echo "  deleting $file"
    rm "$file"
  done <<< "$OLD_OBJECTS"
else
  echo "  (no .o files found)"
fi

if [ -f ./clox ]; then
  echo "  deleting ./clox"
  rm ./clox
else
  echo "  (no old clox executable found)"
fi

echo
echo "🔎 Collecting all C source files..."

mapfile -t C_FILES < <(find c -name "*.c" | sort)

if [ "${#C_FILES[@]}" -eq 0 ]; then
  echo "❌ No C source files found in c/"
  exit 1
fi

echo "🧾 Compiling the following files:"
for file in "${C_FILES[@]}"; do
  echo "  compiling $file"
done

echo
echo "⚙️ Running gcc..."
gcc -Wall -Wextra -Wpedantic -std=c17 -g "${C_FILES[@]}" -o clox

echo
echo "✅ Build complete."
echo "   Executable: ./clox"

WRAPPER="$HOME/.local/bin/clox"
TARGET="$(pwd)/clox"

mkdir -p "$HOME/.local/bin"

cat > "$WRAPPER" <<EOF
#!/usr/bin/env bash
"$TARGET" "\$@"
EOF

chmod +x "$WRAPPER"

echo "🔗 Installed command wrapper at $WRAPPER"