#include "cgi.hpp"


// include request
// include response

CGI::CGI(void)
{
 this->_requestBody = "sdvdfvdfbdfvdfvfgbgbdfvdfb"; //-->request body
  //get current directory pathname: Absolut Path ex: /Users/abboutah/....../websev/
  char * tmp = getcwd(NULL, 0); // arguments 1:current pathname to buf, size of buf
  if (NULL == tmp) 
  {
    return;
  }
  std::string cwd = tmp;
  free(tmp);

  this->_Bin = "/usr/local/bin/php"; //get binary file from request
  if ('/' == this->_Bin[0]) //if absoluth path then okay
  {
    this->_Path = this->_Bin;
  } 
  else //if relative path then convert to absoluth path
  {
    this->_Path = cwd + "/" + this->_Bin;
  }
  this->_TmpPath = random_tmp_file;
  this->_TmpFd = open(this->_TmpPath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00644);
  if (this->_TmpFd <= 0) 
  {
    return ;
  }
  this->_requestFilePath = cwd + "/" + "var/www/default.php"; //get file from response
  return ;
}

CGI::~CGI(void) 
{
  if (this->_TmpFd > 0) 
  {
    close(this->_TmpFd);
    //remove directory entries
    int ret = unlink(this->_TmpPath.c_str());
    if (-1 == ret) 
    {
      std::cerr << "Error! unlink() failed with return code -1 when unlinking: " << this->_TmpPath << std::endl;
    }
  }

  for (size_t i = 0; NULL != this->_av[i]; ++i) 
  {
    free(this->_av[i]);
  }
  free(this->_av);

  for (size_t i = 0; NULL != this->_env[i]; ++i) 
  {
    free(this->_env[i]);
  }
  free(this->_env);

  return ;
}

int CGI::execute(void) 
{
  // env
  int ret = this->initEnv();
  if (0 != ret) 
  {
    return (500); // 500 internal server error
  }
  this->_av = reinterpret_cast<char **>(malloc(3 * sizeof(char *)));
  this->_av[0] = strdup(this->_Path.c_str());   //the path of cgi program or cgi script. Such as “/usr/local/bin/php-cgi”
  this->_av[1] = strdup(this->_requestFilePath.c_str());   //the path of cgi file 
  this->_av[2] = NULL; 
  if (NULL == this->_av[0] || NULL == this->_av[1]) 
  {
    return (500); // 500 internal server error
  }
  int fd[2];
  ret = pipe(fd);
  if (0 != ret) 
  {
    return (500); // 500 internal server error
  }
  pid_t pid = fork();
  if (-1 == pid) 
  {
    return (500); // 500 internal server error
  } 
  else if (pid > 0) //parent
  {
    close(fd[CGI::fRead]); // close read side
    ssize_t aux_ret = write(fd[CGI::fWrite], this->_requestBody.c_str(), this->_requestBody.length()); //write body
    if (-1 == aux_ret) 
    {
      return (500); // 500 internal server error
    }
    close(fd[CGI::fWrite]); //close the end of the pipe 

    int status;
    //WAITPID:  une fonction qui permet de suspendre l'exécution d'un processus père jusqu'à ce qu'un de ses fils, dont on doit passer le PID en paramètre
    // si pid > 0, le processus père est suspendu jusqu'à la fin d'un processus fils dont le PID est égal à la valeur pid ;
    // si pid = 0, le processus père est suspendu jusqu'à la fin de n'importe lequel de ses fils appartenant à son groupe ;
    // si pid = -1, le processus père est suspendu jusqu'à la fin de n'importe lequel de ses fils ;
    // si pid < -1, le processus père est suspendu jusqu'à la mort de n'importe lequel de ses fils dont le GID est égal.
    // Le second argument, status, a le même rôle qu'avec wait.
    // Le troisième argument permet de préciser le comportement de waitpid. On peut utiliser deux constantes :
    // WNOHANG : ne pas bloquer si aucun fils ne s'est terminé.
    // WUNTRACED : recevoir l'information concernant également les fils bloqués si on ne l'a pas encore reçue.
    // Dans le cas où cela ne vous intéresse pas, il suffit de mettre le paramètre 0.
    // Notez que waitpid(-1, status, 0) correspond à la fonction wait.
    ret = waitpid(pid, &status, 0);
    // WIFEXITED(status) Elle renvoie vrai si le statut provient d'un processus fils qui s'est terminé en quittant le main avec return ou avec un appel à exit.
    // WEXITSTATUS(status) Elle renvoie le code de retour du processus fils passé à exit ou à return.
    // Cette macro est utilisable uniquement si vous avez utilisé WIFEXITED avant, et que cette dernière a renvoyé vrai.
    if (-1 == ret || (WIFEXITED(status) && 0 != WEXITSTATUS(status))) 
    {
      return (502); // 502 bad gateway
    }
  } 
  else //child
  {
    if ('/' == this->_requestFilePath[this->_requestFilePath.length() - 1]) //remove the last slach 
    {
      this->_requestFilePath.erase(this->_requestFilePath.length() - 1);
    }
    //chdir:change current working directory
    ret = chdir(this->_requestFilePath.substr(0, this->_requestFilePath.find_last_of('/')).c_str());
    if (-1 == ret) 
    {
      return (500); // 500 internal server error
    }

    close(fd[CGI::fWrite]); //close write side
    ret = dup2(fd[CGI::fRead], STDIN_FILENO); //duplicate the value of read fd to our fd exceve input
    if (-1 == ret) 
    {
      return (500); // 500 internal server error
    }
    close(fd[CGI::fRead]); //close the end of the pipe
    ret = dup2(this->_TmpFd, STDOUT_FILENO); //duplicate the value of write fd to tmpfd  exceve output
    if (-1 == ret) 
    {
      return (500); // 500 internal server error
    }

    if(execve(this->_av[0], this->_av, this->_env) == -1)
    {
      std::cerr << "Error! execve() failed when running:  "<< this->_av[0] << std::endl;      
      exit(-1); // error exit
    }
    exit(1); // error exit
  }

  // read cgi output and save into body
  char* buffer(new char[ (Default_buffer + 1) * sizeof(char)]);
  // lseek:repositions the file offset of the open file description associated with the file descriptor fd to the argument offset
  // The file offset is the position in a file from which data is next read or to which data is next written.
  // SEEK_SET The file offset is set to offset bytes.  
  lseek(this->_TmpFd, 0, SEEK_SET);
  for (ssize_t aux_ret = 0; ;) 
  {
    //buffer.get()
    aux_ret = read(this->_TmpFd, buffer, Default_buffer); //read cgi stdout
    if (0 == aux_ret) 
    {
      break ;
    }
    if (-1 == aux_ret) 
    {
      std::cerr << "Error! read() failed with return code -1"<< std::endl;
      this->_body = "";
      return (500); // 500 internal server error
    }
    buffer[static_cast<size_t>(aux_ret)] = '\0'; //terminate string
    //buffer.get()
    this->_body.insert(this->_body.length(), buffer, static_cast<std::string::size_type>(aux_ret)); //save the output
  }  
  // std::cout << "-------CGI OUTPUT-------" << std::endl;
  // std::cout << this->_body << std::endl;
  return (200); // 200 ok
}

void CGI::parseHeadersAndBody( void)
{

}

int CGI::initEnv(void) 
{
  std::map<std::string, std::string> env;

  // security reasons, to tell the cgi the server handled the request
  env["REDIRECT_STATUS"] = "200"; 
  //  the dialect of CGI being used by the server to communicate with the script //for all requests
  env["GATEWAY_INTERFACE"]  = "CGI/1.1";
  // URI path (not URL-encoded)   which could identify the CGI script (rather than the script's output).
  env["SCRIPT_NAME"]        = this->_Path;
  // the method which should be used by the script to process the request
  env["REQUEST_METHOD"]     = "GET"; //request
  env["REQUEST_URI"]        = this->_requestFilePath;
  // path to be interpreted by the CGI script.
  env["PATH_INFO"]          = this->_requestFilePath;
  // taking the PATH_INFO value, parsing it as a local URI in its own right, and performing any virtual-to-physical 
  // translation appropriate to map it onto the server's document repository structure.
  env["PATH_TRANSLATED"]    = this->_requestFilePath;
  //URL-encoded search or parameter string; it provides information to the CGI script to affect or refine the document to be returned by the script.
  env["QUERY_STRING"]       = ""; //request
  // the network address of the  client sending the request to the server.
  env["REMOTE_ADDR"]        = "127.0.0.1"; // request
  // the name of the server host to which the client request is directed. //for all requests
  env["SERVER_NAME"]        = "127.0.0.1"; // request
  // the TCP/IP port number on which this request is received from the client.
  env["SERVER_PORT"]        = "80"; //request
  // the name and version of the application protocol used for this CGI request.
  env["SERVER_PROTOCOL"]    = "HTTP/1.1"; //Request
  // the name and version of the information server software making the CGI request (and running the gateway). //for all requests
  env["SERVER_SOFTWARE"]    = ""; // empty for security reasons

  if ("POST" == env["REQUEST_METHOD"] || "PUT" == env["REQUEST_METHOD"])
   {
    //contains the size of the message-body attached to the request
    env["CONTENT_LENGTH"] = this->_requestBody.length();
    env["CONTENT_TYPE"] = "text"; //requesy
   }
  //get all request header // revision
  // create **env
  this->_env = reinterpret_cast<char **>(malloc((env.size() + 1) * sizeof(char *)));
  if (nullptr == this->_env) 
  {
    return (-1);
  }
  //convert env to a double array
  char ** aux = this->_env;
  for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it) 
  {
    std::string var = it->first + "=" + it->second;
    *aux = strdup(var.c_str());
    if (NULL == *aux) 
    {
      return (-1);
    }
    ++aux;
  }
  *aux = NULL;
  // std::cout << "-------Meta-variables-------" << std::endl;
  // for(std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); it++)
  //   {
  //       std::cout << it->first << "=" << it->second << std::endl;
  //   }
  //   std::cout << std::endl;

  return (0);
}

  //  Lexer *              _Lexer; //ici define inside the classs
  //   const std::vector<Parser *> * _Servers; // icidefine inside the class
  //   try
  //   {
  //   this->_Lexer = new Lexer(); //ici we create a lexer because the Lexer divide each server
  //   this->_Lexer->start(NULL); // ici  we start Parsing each server
  //   _Servers = _Lexer->getparserv(); // ici we get all servers
  //   }
  //   catch(std::exception & e)
  //       {
  //               std::cerr << e.what() << std::endl;
  //               return ;
  //       }
  //   //ici we iterate server by server
  //   for ( std::vector<Parser *>::const_iterator it = this->_Servers->begin(); it != this->_Servers->end(); ++it )
  //   {
  //   std::map<std::string, Parser *> & locations = (*it)->getLocation(); //ici we get Location  for ech server
  //   for (std::map<std::string, Parser *>::const_iterator b = locations.begin();b != locations.end(); ++b)
  //   {
  //     std::cout << "    " << b->first << std::endl;
  //     b->second->printparser("----Location Block----"); //ici you can print all dir    
  //     // b->second->getx() // ici you can acces to all directives here
  //     } 
