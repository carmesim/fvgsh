

# Parte dos slides

* Introdução: 
    * 1,2,3,4 - Fred
* Tipos de comandos:
    * Simples e Sequencial - Gustavo
    * Lógico - Fred
    * Com pipes - Vinicius
* Implementação:
    * Transição, fork - Vinicius
    * execve - Fred
    * waitpid - Vinícius
    * Syscall pipe() e Go to Demonstração - Gustavo

# Demonstração
* **Fred**
```bash
while true; do clear;pstree;sleep 2; done;
```
# Nossa implementação
* main - Gustavo
    * init_signal_handler - inicia o gerenciador de sinais que recebe sinais como SIGINT e SIGTERM
    * joblist_init - inicializa a lista de trabalhos em background
    * get_user_data - salva nome de usuário, hostname, diretório HOME 
    * print_prompt - printa o prompt ?
    * g_should_exit sinaliza se o shell deve parar sua execução
    * g_waiting_for_child_proc sinaliza se o shell está esperando pelo término de um processo filho
    * Recebe um comando da stdin e o tokeniza
    * joblist_verify - verifica estado dos trabalhos em background
    * define o tipo do comando
    * chama um dos execs de acordo com o tipo do comando
* coisas de string (str_vec, tokenize) - Vinícius
* signal handler - Vinícius
* user_data - Vinícius
* exec.c ?
    * exec_simple_command - Gustavo (cd e exit)
    * exec_seq_command - Fred
    * exec_logical_command - Fred
    * exec_piped_command - Vinícius
* jobs - Fred
   


# Tree
├── basictypes.h
├── command.c
├── command.h
├── exec.c
├── exec.h
├── jobs.c
├── jobs.h
├── main.c  (Gustavo)
├── sighandler.c
├── sighandler.h
├── strutils.c
├── strutils.h
├── strvec.c
├── strvec.h
├── userdata.c
└── userdata.h

