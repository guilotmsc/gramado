;
; File: fs32.inc
;
; Descri��o:
;     Arquivo principal da ger�ncia de sistemas de arquivos em 32bit.
;     Cont�m chamadas � todos os sistemas de arquivos suportados em 32bit.
;     O Mass Storage Manager pode dar suporte b�sico aos sistemas de arquivos 
; mais simples e mais usados.
;     Objetivos: FAT12, FAT16, FAT32, (ELTORITO CD), EXT2, ramfs.
;
; Hist�rico:
;     Vers�o: 1.0, 2015 - Created.
;     Vers�o: 1.0, 2016 - Revis�o.
;     ... 
;



;==============================================================
; fs32Init:
;     Init file system.
;
init_fs:
    nop
fs32Init:
    nop    ;Nothing for now!     
    ret
	
	
;===============================================================
; fs32SaveMetafile:
;     Salvar o metafile para que as altera��es feitas pelo usu�rio
; sejam validadas quando o sistema for reiniciado.
;	
fs32SaveMetafile:
    nop ;;@todo
    ret	
   
;---------------------------------------
; create_partition:
;     Cria uma parti��o.
;
create_partition:
    nop    ;Nothing for now!     
    ret
	
;--------------------------------------
; bootfs_openfile:
;     Abre um arquivo do sistema de arquivos.
;
; eax = endere�o do nome do arquivo
;	
bootfs_openfile:
    call fat16ReadFile
    ret  	
	
	
;=========================================
; fs32ShowActivePartitionInfo:	
;     Mostra parametros da parti��o ativa.
;
fs32ShowActivePartitionInfo:
    nop	
fs32_show_activepartition_info:	
	pushad
	
    mov esi, msg_flagP
	call os_output		 	
	xor eax, eax
	mov al, byte [ParticaoAtiva.flag]
    call print_hex

    mov esi, msg_h_inicialP
	call os_output		
	xor eax, eax
    mov al, byte  [ParticaoAtiva.h_inicial]
    call print_hex	

    mov esi, msg_c_inicialP
	call os_output		
    xor eax, eax
    mov al,  byte  [ParticaoAtiva.c_inicial]
    call print_hex	

    mov esi, msg_s_inicialP
	call os_output		
	xor eax, eax 
    mov al, byte  [ParticaoAtiva.s_inicial]
    call print_hex	

    mov esi, msg_os_typeP
	call os_output		
	xor eax, eax
    mov al, byte  [ParticaoAtiva.os_type]
    call print_hex	

    mov esi, msg_h_finalP
	call os_output		
	xor eax, eax
    mov al, byte  [ParticaoAtiva.h_final]
    call print_hex	

    mov esi, msg_c_finalP
	call os_output		
	xor eax, eax
    mov al, byte  [ParticaoAtiva.c_final]             
    call print_hex	

    mov esi, msg_s_finalP
	call os_output		
    xor eax, eax
	mov al,  byte  [ParticaoAtiva.s_final]             
    call print_hex	
    
    mov esi, msg_lba_inicialP
	call os_output		
	mov eax, dword [ParticaoAtiva.lba_inicial]
    call print_hex	

    mov esi, msg_tamanho_da_particaoP
	call os_output		
    mov eax, dword [ParticaoAtiva.tamanho_da_particao]  
    call print_hex	
	
	;nl
	mov esi, new_line
    call os_output

    popad	
	ret

	
;--------------------------------
; fs32_show_currentdir:
;
;	
fs32_show_currentdir:
pushad
	
	;carrega o primeiro setor apenas.
	
	;Load DIR
	;Inicia ide.
	call init_ide0_master	
	;coloca um setor no buffer.
	mov eax, dword FAT16_ROOTDIR_ADDRESS
	mov ebx, dword [g_current_directory_lba] 
	call hd0_read_sector 
	
	;T�tulo 1 e 2.
	mov esi, dir_title1_string
	call os_output	
	mov esi, dir_title2_string
	call os_output
	
	;pega a base do diretorio do fs do boot
	mov edx, dword FAT16_ROOTDIR_ADDRESS
    
    ;numero de entradas no root dir (32 entradas) de 32 bytes cada
	mov ecx, dword 16
.show_entry:  
	mov esi, edx
	;pega a primeira letra
    mov al, byte [esi]
	;vazia
	cmp al, byte 0
	je .vazia
	;semiapagada
	cmp al, byte '$'
	je .vazia
    ;8.*
	;++
	pushad
    mov ecx, dword 8; contador 
.printchar:
	mov esi, edx ;base
	
	mov al, byte [esi]

	cmp al, byte ' '
	je .nextchar
    
    ;se for espa�o nao imprime
	push ecx
	mov ecx, dword 1 
	call os_output_chars
    pop ecx
	
.nextchar:
	add edx, dword 1
	loop .printchar

.chardone:
    mov esi, msg_fat16_Progress
    call os_output
	popad
	;--	
	
	;*.3
	;++
	pushad
	add edx, dword 8
	mov esi, edx
	mov ecx, dword 3 
	call os_output_chars
	popad
	;--
	
	;espa�o
    mov esi, msg_fat16_space
	call os_output
		
    ;lba inicial
	pushad
	mov word [os_Screen_Cursor_Col], 24
	;mov word [os_Screen_Cursor_Row], 0	
	
	xor eax, eax
	mov al, byte [edx + 0x1A] ;;;lba inicial
    call print_hex ; imprime flag em hexa	
    popad

	;s� pula linha se nao for vazia.
	;new line
	mov esi, new_line
	call os_output	
	
.vazia:	
	add edx, 32    ;Pr�xima entrada.
	dec ecx
	
	cmp ecx, dword 0
	je .exit_d
	jmp .show_entry
.exit_d:	
	popad
	ret		

	
;-------------------------------
; fs32_show_rootdir:
;
fs32_show_rootdir:
 
	pushad
	
	;carrega o primeiro setor apenas.
	call load_root_first_sector 
	
	;T�tulo 1 e 2.
	mov esi, dir_title1_string
	call os_output	
	mov esi, dir_title2_string
	call os_output
	
	;pega a base do diretorio do fs do boot
	mov edx, dword FAT16_ROOTDIR_ADDRESS
    
    ;numero de entradas no root dir (32 entradas) de 32 bytes cada
	mov ecx, dword 16
.show_entry:
    
	mov esi, edx
	
	;pega a primeira letra
    mov al, byte [esi]
	
	;vazia
	cmp al, byte 0
	je .vazia
	
	;semiapagada
	cmp al, byte '$'
	je .vazia
	
    ;8.*
	;++
	pushad
    mov ecx, dword 8; contador 
.printchar:
	mov esi, edx ;base
	
	mov al, byte [esi]

	cmp al, byte ' '
	je .nextchar
    
    ;se for espa�o nao imprime
	push ecx
	mov ecx, dword 1 
	call os_output_chars
    pop ecx
	
.nextchar:
	add edx, dword 1
	loop .printchar

.chardone:
    mov esi, msg_fat16_Progress
    call os_output
	popad
	;--	
	
	;*.3
	;++
	pushad
	add edx, dword 8
	mov esi, edx
	mov ecx, dword 3 
	call os_output_chars
	popad
	;--
	
	;espa�o
    mov esi, msg_fat16_space
	call os_output
		
    ;lba inicial
	pushad
	mov word [os_Screen_Cursor_Col], 24
	;mov word [os_Screen_Cursor_Row], 0	
	
	xor eax, eax
	mov al, byte [edx + 0x1A] ;;;lba inicial
    call print_hex ; imprime flag em hexa	
    popad

	;s� pula linha se nao for vazia.
	;new line
	mov esi, new_line
	call os_output	
.vazia:	
	;pr�xima entrada
	add edx, 32
	
	dec ecx
	
	cmp ecx, dword 0
	je .exit_d
	
	jmp .show_entry
.exit_d:	
	popad
	ret
	
	
	
; Continua ...	


;Parti��o ativa.
;@todo: Melhorar.
msg_flagP                db 13, 10,  "Flag        - ", 0
msg_h_inicialP           db 13, 10,  "Initial H   - ", 0
msg_c_inicialP           db 13, 10,  "Initial C   - ", 0
msg_s_inicialP           db 13, 10,  "Initial S   - ", 0
msg_os_typeP             db 13, 10,  "Os type     - ", 0
msg_h_finalP             db 13, 10,  "final H     - ", 0
msg_c_finalP             db 13, 10,  "final C     - ", 0
msg_s_finalP             db 13, 10,  "final S     - ", 0
msg_lba_inicialP         db 13, 10,  "Initial LBA - ", 0
msg_tamanho_da_particaoP db 13, 10,  "P size      - ", 0 
	
	
;
;End.
;

