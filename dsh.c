//creazione schell dsh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 4096
#define MAX_ARGS 256
#define MAX_PATH 512
#define MAX_PROMPT 32
//path attuale globale
char _path[MAX_PATH]="/bin/:/usr/bin/";

void panic(const char* msg){
    if(errno){
        fprintf(stderr,"PANIC: %s %s \n\n", msg, strerror(errno));
    }else{
        fprintf(stderr,"PANIC %s\n\n",msg);
    }
    exit(EXIT_FAILURE);
}

int  prompt(char* buf, size_t buf_size, const char* prompt_string){
    //print dsh$
    printf("%s ", prompt_string);
    if(fgets(buf, buf_size, stdin)==NULL){
        return EOF;
    }
    size_t cur = strlen(buf);
    if (buf[cur - 1] == '\n')
    {
        buf[cur - 1] = '\0';
    }
    return cur;
}

void set_path(const char* new_path){
    if(new_path != NULL){
#if USE_DEBUG_PRINTF
	printf("DEBUG: new_path: %s\n", new_path);
#endif
        //sostituiamo il path
            int new_path_len=0;
            while(new_path[new_path_len] != '\0'){
                new_path_len++;
                if(new_path_len >=  MAX_PATH -1 && new_path[new_path_len] != '\0'){
                    fprintf(stderr, "ERROR: path_string too long");
                    return;
                }
            }
            if(new_path_len > 0){
            	memcpy(_path, new_path, new_path_len+1);
            	}
    }else{
    //stampa il path attuale
    	printf("%s\n", _path);
    }
}

//eseguo comandi specificati nel path
void path_lookup(char* abs_path, const char* rel_path ){
	char* prefix;
	char buf[MAX_PATH];
	if(abs_path==NULL || rel_path == NULL)
		panic("errors in the parameters abs_path or rel_path");
	prefix=strtok(_path, ":");
	while(prefix != NULL){
		strcpy(buf, prefix);
		strcat(buf, rel_path);
		if(access(buf, X_OK)==0){
			strcpy(abs_path, buf);
			return;
		}
		prefix=strtok(NULL, ":");
	}
	strcpy(abs_path, rel_path);
}
void rel2abs(char** arg_list){
	if(arg_list[0][0] == '/'){
    		//assume path assoluto
    	execv(arg_list[0], arg_list);
    }else{
    	//assume path relativo
    	char abs_path[MAX_PATH];
        path_lookup(abs_path, arg_list[0]);
        execv(abs_path, arg_list);
    	}
}

void do_redir(const char* out_path, char** arg_list, const char* mode){
	if(out_path == NULL){
    	panic("FILE OUTPUT NOT SPECIFIED");
    }
	   int pid=fork();
    if(pid > 0){
        int wpid = wait(NULL);
        if(wpid < 0) panic("do_redir wait");
    } else if(pid==0){
    	FILE* out=fopen(out_path, mode);
    	if(out == NULL){
    		perror(out_path);
    		exit(EXIT_FAILURE);
    	}
    	dup2(fileno(out), 1);//1 è lo standard output
        fclose(out);
        rel2abs(arg_list);
        perror(arg_list[0]);
        exit(EXIT_FAILURE);	
        //end child code
    } else{
        panic("do_redird fork");
    }
}

void do_pipe(size_t pipe_pos, char** arg_list){
    int pipefd[2];
    int pid;
    if (pipe(pipefd) < 0) panic("do_pipe pipe"); 
    // parte sinistra della pipe
    pid = fork();
    if (pid > 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        int wpid = wait(NULL);
        if (wpid < 0)
            panic("do_pipe wait");
    }
    else if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        rel2abs(arg_list);
        perror(arg_list[0]);
        exit(EXIT_FAILURE);
        // end child code
    }
    else
    {
        panic("do_pipe fork");
    }
    //parte destra della pipe
    pid = fork();
    if (pid > 0)
    {
        int wpid = wait(NULL);
        if (wpid < 0)
            panic("do_pipe wait");
    }
    else if (pid == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        rel2abs(arg_list + pipe_pos + 1);
        perror(arg_list[pipe_pos + 1]);
        exit(EXIT_FAILURE);
        // end child code
    }
    else
    {
        panic("do_pipe fork");
    }
}


void do_exec(char** arg_list){
    int pid=fork();
    if(pid > 0){
        int wpid = wait(NULL);
        if(wpid < 0) panic("do_exec wait");
    } else if(pid==0){
    	rel2abs(arg_list);
        perror(arg_list[0]);
        exit(EXIT_FAILURE);	
        //end child code
    } else{
        panic("do_exec fork");
    }
}

int main(void)
{
    char input_buffer[MAX_LINE];
    size_t arg_count;
    char *arg_list[MAX_ARGS + 1]; // Increase size by 1 for null termination
    char prompt_string[MAX_PROMPT] = "\0";
    if (isatty(0))
    {
        strcpy(prompt_string, "dsh$\0");
    }
    while (prompt(input_buffer, MAX_LINE, prompt_string) >= 0)
    {
        // tokenize the input line
        // usansando la funzione strtok
        arg_count = 0;
        arg_list[arg_count] = strtok(input_buffer, " ");
        if (arg_list[arg_count] == NULL)
        {
            // non viene scritto nulla nel prompt e viene schiacciato invio
            continue;
        }
        else
        {
            do
            {
                arg_count++;
                if (arg_count > MAX_ARGS)
                    break;
                arg_list[arg_count] = strtok(NULL, " ");
            } while (arg_list[arg_count] != NULL);
        }
        arg_list[arg_count] = NULL; // Null terminate the arg_list

        // interprete dei built in
        // exit
        if (strcmp(arg_list[0], "exit") == 0)
        {
            break;
        }
        // path built in
        if (strcmp(arg_list[0], "setpath") == 0)
        {
            set_path(arg_list[1]);
            continue;
        }
        {
            size_t redir_pos = 0;
            size_t append_pos = 0;
            size_t pipe_pos = 0;
            // check for special caracters
            for (int i = 0; i < arg_count; i++)
            {
                if (strcmp(arg_list[i], ">") == 0)
                {
                    redir_pos = i;
                    break;
                }
                if (strcmp(arg_list[i], ">>") == 0)
                {
                    append_pos = i;
                    break;
                }
                if (strcmp(arg_list[i], "|") == 0)
                {
                    pipe_pos = i;
                    break;
                }
            }
            if (redir_pos != 0)
            {
                arg_list[redir_pos] = NULL;
                // effetuo la ridirezione
                do_redir(arg_list[redir_pos + 1], arg_list, "w+");
            }
            else if (append_pos != 0)
            {
                arg_list[append_pos] = NULL;
                // effettuo la ridirezione
                do_redir(arg_list[append_pos + 1], arg_list, "a+");
            }
            else if (pipe_pos != 0)
            {
                arg_list[pipe_pos] = NULL;
                do_pipe(pipe_pos, arg_list);
            }
            else
            {
                // exec
                do_exec(arg_list);
            }
        }
    }
    // puts("");
    exit(EXIT_SUCCESS);
}