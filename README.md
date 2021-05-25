# unix-shell

	├── fvgsh.pro	          Arquivo QMake que contrói esse projeto
	├── LICENSE               A licença MIT
	├── README.md             O arquivo que você está lendo
	└── src            
	    ├── main.c            
	    ├── basictypes.h      Tipos básicos e constantes utilizadas pelo projeto
	    ├── sighandler.h/.c   Define o signal handler (tratamento de sinais como SIGINT)
	    ├── strutils.h/.c     Funções utilitárias para strings
	    ├── exec.h/.c         Funções utilitárias  para execução de comandos
	    ├── jobs.h/.c         Tipos e funções para lidar com processos em segundo plano
	    ├── strvec.h/.c      
	    └── userdata.h/.c     Struct e funções que armazenam alguns dados do usuário
