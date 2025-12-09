QEMUFLAGS ?= -M q35,smm=off -m 8G -cdrom willowos.iso -serial stdio -smp 4

.PHONY: all
all:
	rm -f willowos.iso
	$(MAKE) willowos.iso

willowos.iso: jinx
	./build-support/makeiso.sh

.PHONY: debug
debug:
	JINX_CONFIG_FILE=jinx-config-debug $(MAKE) all

jinx:
	git clone https://codeberg.org/mintsuki/jinx.git jinx-repo
	git -C jinx-repo checkout b3c7da97e5247bee0a876a7a5f6c104f019fcf79
	mv jinx-repo/jinx ./
	rm -rf jinx-repo

.PHONY: run-kvm
run-kvm: willowos.iso
	qemu-system-x86_64 -enable-kvm -cpu host $(QEMUFLAGS)

.PHONY: run-hvf
run-hvf: willowos.iso
	qemu-system-x86_64 -accel hvf -cpu host $(QEMUFLAGS)

ovmf/ovmf-code-x86_64.fd:
	mkdir -p ovmf
	curl -Lo $@ https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-code-x86_64.fd

ovmf/ovmf-vars-x86_64.fd:
	mkdir -p ovmf
	curl -Lo $@ https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/ovmf-vars-x86_64.fd

.PHONY: run-uefi
run-uefi: willowos.iso ovmf/ovmf-code-x86_64.fd ovmf/ovmf-vars-x86_64.fd
	qemu-system-x86_64 \
		-enable-kvm \
		-cpu host \
		-drive if=pflash,unit=0,format=raw,file=ovmf/ovmf-code-x86_64.fd,readonly=on \
		-drive if=pflash,unit=1,format=raw,file=ovmf/ovmf-vars-x86_64.fd \
		$(QEMUFLAGS)

.PHONY: run-bochs
run-bochs: willowos.iso
	bochs -f bochsrc

.PHONY: run-lingemu
run-lingemu: willowos.iso
	lingemu runvirt -m 8192 --diskcontroller type=ahci,name=ahcibus1 --disk willowos.iso,disktype=cdrom,controller=ahcibus1

.PHONY: run
run: willowos.iso
	qemu-system-x86_64 $(QEMUFLAGS)

.PHONY: clean
clean:
	rm -rf iso_root sysroot willowos.iso initramfs.tar

.PHONY: distclean
distclean: clean
	make -C kernel distclean
	rm -rf .jinx-cache jinx builds host-builds host-pkgs pkgs sources ovmf
