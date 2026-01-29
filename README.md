# G-TomOS

G-TomOS is a minimalist educational operating system written in C. It boots through GRUB and displays the text prompt `G-TomOS:>` on the screen.

---

## Project Structure

```

G-TomOS/
├── bin/                 # Compiled binary files (gtomos.elf)
├── src/
│   ├── linker.ld        # Linker script for the kernel
│   └── kernel/
│       ├── kernel.c
│       └── io.h
└── Makefile             # Kernel build script

```

---

## Build and Run

1. Compile the kernel:  
```bash
make
```

2. Create a GRUB ISO:

```bash
mkdir -p iso/boot/grub
cp bin/gtomos.elf iso/boot/
cp grub.cfg iso/boot/grub/
grub-mkrescue -o gtomos.iso iso
```

3. Run in QEMU:

```bash
qemu-system-i386 -cdrom gtomos.iso
```

---

## File Description

* `io.h` — low-level input/output port functions
* `kernel.c` — main kernel and VGA driver
* `linker.ld` — kernel memory layout
* `Makefile` — build automation script
