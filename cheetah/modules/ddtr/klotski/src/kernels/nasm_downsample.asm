;
; The MIT License (MIT)
;
; Copyright (c) 2016-2023 The SKA organisation
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.
;

SIZE_OF_YMM equ 32
SIZE_OF_ZMM equ 64

GLOBAL nasm_downsample
nasm_downsample:

        ;rdi: DataIn
        ;rsi: number_of_elements

        mov eax, 0x0000ffff
        movd xmm5, eax
        vpbroadcastd zmm5, xmm5

        mov eax, 0xffff0000
        movd xmm6, eax
        vpbroadcastd zmm6, xmm6

        mov eax, 2
        movd xmm7, eax
        vpbroadcastd zmm7, xmm7
        vcvtdq2ps zmm7, zmm7

        xor r10, r10 ; set element = 0
element_loop:
        mov rax, SIZE_OF_ZMM
        mul r10
        vmovdqu16 zmm1, zword [rdi+rax]
        vpandd zmm3, zmm1, zmm5
        vpandd zmm4, zmm1, zmm6
        vpsrad zmm4, zmm4, 0x10
        vpaddd zmm3, zmm3, zmm4
        vcvtdq2ps zmm3, zmm3
        vdivps zmm3, zmm3, zmm7
        vcvtps2udq zmm3, zmm3, {ru-sae}
        vpmovdw ymm3, zmm3
        mov rax, SIZE_OF_YMM
        mul r10
        vmovdqu yword [rdi+rax], ymm3
        inc r10
        mov rax, 32
        mul r10
        cmp rax, rsi
        jl element_loop
        ret