 
#include "shell.h" 


char xxxusername[11];
char xxxpassword[11];

extern int login_status;


int loginCheckPassword (){
	
	// #importante
	// >>>> suspenso <<<
	// problemas no input de char ,,,
	
	return -1;
	
    //buffer para o arquivo todo.
	char buffer[512];	
	
	char buffer_username[32];	
	char buffer_password[32];	
	
	
	int i;
	
	// Somente um shell interativo tem login.
	
		
		
		//file 
		
		FILE *user_stream;
		
		user_stream = (FILE *) fopen ("user.txt","w+");
		
		//#todo check.
		if ( (void *) user_stream == NULL )
		{
		    printf ("Couldn't open the file\n");
		}
		
		// Testing welcome message.
	    //printf("\n");
	    printf("\n Welcome to Gramado shell! \n");
	    //printf("\n");
	
		//#obs
		//gets is on (stdio.c)
        		
		//
		//  ## username  ##
		//
		
	    printf("\n username: ");
	    gets(xxxusername);
		
 
		//
		//  ## password ##
	    //
		
		printf("\n password: ");
	    gets(xxxpassword);
		
		printf("\n");
	
//#ifdef SHELL_VERBOSE		
	    printf("username={%s} password={%s} \n", xxxusername, xxxpassword );
		//printf("\n");
//#endif
		
		//===================================
		
		//#importante
		//username e passowrd contem as digitaçoes.
		
		//vamos checar o conteudo do arquivo.
		
		//char *c = (char *) &user_stream->_base[0];		
		
		char *c;
		int ch;
		i = 0;
		
		//Pega todo o arquivo e coloca no buffer[].
        while (1)
		{
			//#bugbug: page fault quando chamamos fgetc.
			//printf("1");
			//ch_test = (int) fgetc (f1);
			//ch_test = (int) getc (f1); 
			
			ch = (int) fgetc (user_stream);
			
			if ( ch == EOF )
			{
				printf("\n\n");
				printf("EOF reached :)\n\n");
				//goto exit_cmp;
				goto out;
				
			}else{
				//printf("2");
			    printf("%c", ch );
				
				//salva.
				buffer[i] = (char) ch;
				i++;
			};
		};		
		
out:	
		//in'icio do buffer.
		c = &buffer[0];
		
		while ( *c && *c != 'P' ){			
			c++;
		};
		
		if ( c[0] == 'U' &&
             c[1] == 'S' &&
             c[2] == 'E' &&
             c[3] == 'R' &&
             c[4] == 'N' &&
             c[5] == 'A' &&
             c[6] == 'M' && 			
		     c[7] == 'E' )
		{
		    //USERNAME={fred}
            c = c+10; 
			
		    
			//Move apenas 'fred'
		    for ( i=0; i<4; i++ ){
			    buffer_username[i] = c[i];
		    }			
			
			//printf("\n");
			//printf("%c", c[0]);
			//printf("%c", c[1]);
			//printf("%c", c[2]);
			//printf("%c", c[3]);
			//printf("\n");
			
			//printf("\n");
			//printf("%c", buffer[0]);
			//printf("%c", buffer[1]);
			//printf("%c", buffer[2]);
			//printf("%c", buffer[3]);
			//printf("\n");
			
//#ifdef SHELL_VERBOSE			
			printf(">>%s\n", xxxusername);
			printf(">>%s\n", buffer_username);
//#endif
			
            if ( strncmp( xxxusername, buffer_username, 4 ) == 0 )
            {
//#ifdef SHELL_VERBOSE				
				printf("# USERNAME OK #\n");
//#endif				
				login_status = 1;
			}else{
				printf("# USERNAME FAIL #\n");
				login_status = 0;
			};				

        }else{
			
			printf("# USERNAME FAIL #\n");
			login_status = 0;
		};


		while ( *c && *c != 'P' ){			
			c++;
		};
		
		if ( c[0] == 'P' &&
             c[1] == 'A' &&
             c[2] == 'S' &&
             c[3] == 'S' &&
             c[4] == 'W' &&
             c[5] == 'O' &&
             c[6] == 'R' && 			
		     c[7] == 'D' )
		{
		    //PASSWORD={1234}
            c = c+10; 
			
		    //Move apenas '1234'
		    for ( i=0; i<4; i++ ){
			    buffer_password[i] = c[i];
		    };

//#ifdef SHELL_VERBOSE				
			printf(">>%s\n", xxxpassword);
			printf(">>%s\n", buffer_password);
//#endif			
			
            if ( strncmp( xxxpassword, buffer_password, 4 ) == 0 )
            {
//#ifdef SHELL_VERBOSE								
				printf("# PASSWORD OK #\n");
//#endif
				login_status = 1;
			}else{
				printf("# PASSWORD FAIL #\n");
				login_status = 0;
			};					
			
		}else{
			
		    printf("# PASSWORD FAIL #\n");
            login_status = 0; 			
		};
	
//#ifdef SHELL_VERBOSE
		printf("Login done!\n");
//#endif
	
	return (int) login_status;
}


