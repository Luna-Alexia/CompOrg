#Luna
#Assignment3 - IntroCompOrg



# global variables
//initialize global data
    .section .data  # stores global variables within the .data part of memory, 
    .globl state # allows staet to be a global variable
    .align 4 # declares a 4byt boundary

    .bss # part of uninitialized data
    .globl input
    state: .long 0
    input: .zero 1



.section .rodata
JT:                     # relative jump table
        .quad Q0 - JT   # case Q0 is at this address + JT
        .quad Q1 - JT   # case Q1 is at this address + JT
        .quad Q2 - JT   # case Q2 is at this address + JT
        .quad Q3 - JT   # case Q3 is at this address + JT


prompt:
    .string "Enter a string of 0s and 1s: "

error:
    .string "ERROR: invalid input"

accepted:
    .string "Input Accepted"
rejects:
    .string "Input rejected"


.section .text
.globl main
main: 
    # makes base pointer/organize stack
    push %rbp 
    movq %rsp, %rbp
    leaq state(%rip), %rbx # loading variable pointer to rbx
    leaq input(%rip), %r12 # loading variable pointer to r12
    movl $0, (%rbx) # intialize state to zero
    
    # output prompt
    leaq prompt(%rip), %rdi # loads prompt from above
    call puts # prints prommpt
    jmp while 
    
while:
    # collecting user inputs
    call getchar
    movb %al, (%r12) # storing input
    cmpb $10, (%r12) # checks of its \n (ascii code)
    jne checker # jmp if not equal, goes to checker fxn if not \n

    #  continues if \n
    movl (%rbx), %eax # loads state into eax
    cmpl $0, %eax # checks state to 0/Q0
    je good # jump if equal, moves to good fxn
    jne bad # jumps if not equal, goes to bad fxn
    



checker:
    cmpb $48, (%r12) # compares input to 0
    je switch
    cmpb $49, (%r12) # compare to one
    je switch
    leaq error(%rip), %rdi # loads string variable
    call puts # print
    jmp exit # moves to exit fxn

switch:
    movl (%rbx), %eax # load state into eax
    leaq JT(%rip), %r13 # stores address of jump table
    movslq (%r13, %rax, 8), %r14 # loads adress of the next state from jt
    addq %r13, %r14 # changing absolute address 
    jmp *%r14 # jumps to directed case



# cases
Q0:
    cmpb $48, (%r12) # compare zero
    movl $0, %ecx # seting ecx to 0
    cmove %ecx, %eax # if input is 0, state = Q0
    movl $1, %ecx # loads address of next jt
    cmovne %ecx, %eax # if not 0, then state is q1
    jmp updater

Q1:
    cmpb $48, (%r12) # compare zero
    movl $2, %ecx # setting to two for q2
    cmove %ecx, %eax # if input is 0, state = q0
    movl $3, %ecx # setting 3 for q3
    cmovne %ecx, %eax # if not 0, state is q3
    jmp updater

Q2:
    cmpb $48, (%r12) # comparing zero
    movl $0, %ecx # zero for q0
    cmove %ecx, %eax # if input is zero, move to q0
    movl $3, %ecx # setting 3 for q3
    cmovne %ecx, %eax # if input not 0, then state is q3
    jmp updater

Q3:
    cmpb $48, (%r12) # comparing inputs for to zero
    movl $2, %ecx # setting 2 for q2
    cmove %ecx, %eax # if input is zero, state is q2
    movl $3, %ecx # setting 3 for q3
    cmovne %ecx, %eax # if input not 0, then state is q3
    jmp updater


updater:
    movl %eax, (%rbx) # updates state
    jmp while

good:
    leaq accepted(%rip), %rdi # stores accepted message
    call puts # calls put to print message
    jmp exit

bad:
    leaq rejects(%rip), %rdi # stores rejects message
    call puts # call put to print message
    jmp exit

exit: # clearing the stack
    popq %rbx 
    popq %r12
    movq %rbp, %rsp
    popq %rbp
    ret
