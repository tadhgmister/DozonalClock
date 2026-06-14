;; manifest.scm — Guix shell environment for Arduino Nano (ATmega328P) development
;;
;; Usage:
;;   guix shell --manifest=manifest.scm
;;   -- or simply --
;;   guix shell   (if this file is named manifest.scm in the project root)

(specifications->manifest
 '(;; Complete AVR toolchain: avr-gcc, avr-binutils, avr-libc, avrdude
   "gcc-cross-avr-toolchain"
   "avrdude"

   ;; GNU make for running the Makefile
   "make"))
