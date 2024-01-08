#include "cgi.hpp"

// kill -9 $(lsof -ti:5000,5001,5002)
CGI::CGI(  HttpRequestParser & Request, Response & Response ) : _Request(Request), _Response(Response)
{
  
 this->_requestBody = this->_Request.bodyFileName; //-->request body
  //get current directory pathname: Absolut Path ex: /Users/abboutah/....../websev/
  char * tmp = getcwd(NULL, 0); // arguments 1:current pathname to buf, size of buf
  if (NULL == tmp) 
  {
    return;
  }
  std::string cwd = tmp;
  free(tmp);

  this->_Bin = this->_Response._cgi_bin; //get binary file from request
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
  this->_requestFilePath = cwd + "/" + Response._file_path; //get file from response
  std::cout << this->_requestFilePath << std::endl;
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
  std::cout << "cgi here" << std::endl;
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
    ret = waitpid(pid, &status, 0); //WNOHANG
    std::cout << "return: " << ret << std::endl;
      // waitpid(pid, &status, WNOHANG); //WNOHANG
    // WIFEXITED(status) Elle renvoie vrai si le statut provient d'un processus fils qui s'est terminé en quittant le main avec return ou avec un appel à exit.
    // WEXITSTATUS(status) Elle renvoie le code de retour du processus fils passé à exit ou à return.
    // Cette macro est utilisable uniquement si vous avez utilisé WIFEXITED avant, et que cette dernière a renvoyé vrai.
    // WIFSIGNALED(status) returns true if the child process was terminated by a signal.
    // WTERMSIG(status)  returns the number of the signal that caused the child process to terminate. This macro should only be employed if WIFSIGNALED returned true.
    if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGALRM))
      return (408);
    // if (WIFEXITED(status) && 0 != WEXITSTATUS(status)) 
    if (-1 == ret || (WIFEXITED(status) && 0 != WEXITSTATUS(status))) 
        return (502); // 502 bad gateway
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
    // struct rlimit a;
    // a.rlim_cur = 10;
    // a.rlim_max = 15;
    // if (setrlimit(RLIMIT_CPU, &a) == 0)
    // {
    //   signal(SIGALRM, timeout_handler);
    //   alarm(10);  // Set the alarm for 10 seconds

    //   // Your existing code for executing the CGI program

    //   // Disable the alarm if the child process finishes before the timeout
    //   alarm(0);
    // }
    alarm(5);
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
  std::cout << "-------CGI OUTPUT-------" << std::endl;
  std::cout << this->_body << std::endl;
  return (200); // 200 ok
}

std::string & CGI::lefttrim(std::string & s, const char * t = " \t\n\r\f\v") 
{
  std::string::size_type index = s.find_first_not_of(t);
  if (std::string::npos == index) 
  {
    return (s);
  }
  s.erase(0, index);
  return (s);
}

std::string & CGI::righttrim(std::string & s, const char * t = " \t\n\r\f\v") 
{
  std::string::size_type index = s.find_last_not_of(t);
  if (std::string::npos == index) 
  {
    return (s);
  }
  s.erase(index + 1);
  return (s);
}

// trim from left & right
std::string & CGI::trim(std::string & s, const char * t = " \t\n\r\f\v") 
{
  return (lefttrim(righttrim(s, t), t));
}

void CGI::parseHeadersAndBody(std::string & body) 
{
  std::string key, value;
  std::string::size_type sep;
  
  // iterate line by line //revision
  for (std::string::size_type eol = this->_body.find(end_of_file); eol != std::string::npos; eol = this->_body.find(end_of_file)) 
  {
    //if no more headers (two consecutive new lines)
    if (0 == eol) 
    {
      this->_body.erase(0, eol + end_of_file.length());
      break ;
    }

    // parse headers // revision
    sep = this->_body.find(':'); 
    if (std::string::npos == sep || 0 == sep) 
    {
      this->_body.erase(0, eol + end_of_file.length());
      continue ;
    }
    key   = this->_body.substr(0, sep);
    std::cout << "%KEY" << key << std::endl;
    value = this->_body.substr(sep + 1, eol - sep - 1);
    if(key == "Content-type" || key == "Content-Type")
      this->_type = trim(value,  "\r\n");
    // std::cout << "%VALUE" << value << std::endl;
    // if (headers.end() != headers.find(key)) 
    // {
    //   this->_body.erase(0, eol + end_of_file.length());
    //   continue ;
    // }
    // headers[trim(key, " \r\n")] = trim(value, " \r\n");    
    //  if (true == headers[key].empty()) 
    // {
    //   headers.erase(key);
    // }
    this->_body.erase(0, eol + end_of_file.length());
  }

  // security reason exploiting 'stack-smashing' or 'stack-overflow' vulnerabilities of the operating system.
  // revision
  // std::map<std::string, std::string>::const_iterator contentLength = headers.find("Content-Length");
  // if (headers.end() != contentLength) 
  // {
  //   this->_body.erase(static_cast<std::string::size_type>(atoi(contentLength->second.c_str())));
  // }
  // headers["Content-Length"] =convert_to_string(this->_body.length());
  this->_length = convert_to_string(this->_body.length());
  // // body
  body = this->_body;
  // printf("______CGI BODY_____\n%s\n", body.c_str());
}


int CGI::initEnv(void) 
{
  std::map<std::string, std::string> env;

  // security reasons, to tell the cgi the server handled the request
  //  the dialect of CGI being used by the server to communicate with the script //for all requests
  // URI path (not URL-encoded)   which could identify the CGI script (rather than the script's output).
  
  
  
  
  
  
  
  
    // env["REQUEST_URI="] + request.getPath().substr(0, request.getPath().find_last_of("/")));
    // env["SCRIPT_NAME="] +  request.getPath());
    // env["PATH_TRANSLATED="] + response.get_full_path());
    // env["QUERY_STRING="] + request.getQueryString());
    // env["REQUEST_METHOD="] + request.getMethod());
    // env["GETAWAY_INTERFACE=CGI]/1.1");
    // env["REDIRECT_STATUS=200]");
    // env["SERVER_SOFTWARE=webserv]/1.1");
    // env["REMOTE_ADDR=0].0.0.0");
    // env["REMOTE_PORT=0]");
    // env["SERVER_PROTOCOL"]    = this->_Request.getVersion(); //Request
  
  
  
  
  
    env["REQUEST_URI"]        = this->_requestFilePath;
    env["SCRIPT_NAME"]        = this->_Path;
  env["PATH_TRANSLATED"]    = this->_requestFilePath;
  env["QUERY_STRING"]       = this->_Request.getQuery(); //request
  env["REQUEST_METHOD"]     = this->_Request.getMethod(); //request
  env["GATEWAY_INTERFACE"]  = "CGI/1.1";
  env["REDIRECT_STATUS"] = "200"; 
  env["SERVER_SOFTWARE"]    = ""; // empty for security reasons
  env["PATH_INFO"]          = this->_requestFilePath;













  // the method which should be used by the script to process the request
  // path to be interpreted by the CGI script.
  // taking the PATH_INFO value, parsing it as a local URI in its own right, and performing any virtual-to-physical 
  // translation appropriate to map it onto the server's document repository structure.
  //URL-encoded search or parameter string; it provides information to the CGI script to affect or refine the document to be returned by the script.
  // the network address of the  client sending the request to the server.
  // env["REMOTE_ADDR"]        = "127.0.0.1"; // request
  // // the name of the server host to which the client request is directed. //for all requests
  // env["SERVER_NAME"]        = "127.0.0.1"; // request
  // // the TCP/IP port number on which this request is received from the client.
  // env["SERVER_PORT"]        = "80"; //request
  // the name and version of the application protocol used for this CGI request.
  // the name and version of the information server software making the CGI request (and running the gateway). //for all requests

  if (env["REQUEST_METHOD"] == "POST")
   {
    //contains the size of the message-body attached to the request
    std::cout << " Content Length request" <<  this->_Request.getHeadersMap().find("Content-Length")->second << std::endl;
    env["CONTENT_LENGTH"] = this->_Request.getHeadersMap().find("Content-Length")->second;
    env["CONTENT_TYPE"] = this->_Request.getHeadersMap().find("Content-Type")->second; //requesy
   }
  else
    {
      env["CONTENT_LENGTH"] = "0";
      env["CONTENT_TYPE"] = "text/html; charset=UTF-8"; //requesy
    }
  //get all request header // revision
  //get all request header and add HTTP + toupper

		std::map<std::string, std::string> headers = this->_Request.getHeadersMap();

  for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) 
  {
    if (false == it->second.empty()) 
    {
      std::string header = it->first;
      std::transform(it->first.begin(), it->first.end(), header.begin(), static_cast<int(*)(int)>(std::toupper));
      std::replace(header.begin(), header.end(), '-', '_');
      header.insert(0, "HTTP_");
      env[header] = it->second;
    }
  }
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

std::string	CGI::convert_to_string(size_t a)
{
	std::stringstream tmp;

	tmp << a;

	return tmp.str();
}

//    //iterate cgi extensions one by one to find the exact extension of the file
// for ( std::map<std::string, std::string>::const_iterator it = getcgi().begin(); it != getCgi().end(); ++it ) {
//      //compare the extension of cgi with the extension of the file
//     if ( getextension() == it->first ) 
//      {
//     CGI a =  new Cgi(request, response)); call cgi and fill the parameters with the data of request and response 

//     int b = a->execute(); //execute cgi and return status code
//     if ( b >= 400 ) //check failure
//      {
//       this->_statuscode = b;
//       return (this->_statuscode);
//     }

//     a->parseHeadersAndBody( this->_headers, this->_body ); //I parse here the header and send you the cgi output in your body
//     this->_headers["Content-Length"] = this->_body.length(); //update the content length

//     this->_statusCode = b;
//     return (this->_statuscode);
//   }
// }
