<?php
//  session is a way to store information (in variables) to be used across multiple pages.
//  Unlike a cookie, the information is not stored on the users computer.
// on the internet there is one problem: the web server does not know who you are or what you do,
//  because the HTTP address doesn't maintain state
// Session variables solve this problem by storing user information to be used across multiple pages and 
// it last until the user closes the browser
// Session variables hold information about one single user, and are available to all pages in one application.


// Start the session
session_start();
# A cookie is often used to identify a user. A cookie is a small file that the server embeds on the user's computer. 
# Each time the same computer requests a page with a browser, it will send the cookie too. With PHP,
# you can both create and retrieve cookie values.
# A cookie is created and modified with the setcookie(name, value, expire, path, domain, secure, httponly);
# To delete a cookie, use the setcookie() function with an expiration date in the past: time() - xxxx
$cookie_name = "user";
$cookie_value = "John Doe";
#The cookie will expire after 30 days 86400s = 1 day, 3600s = 1 hour 
# The "/" means that the cookie is available in entire website
setcookie($cookie_name, $cookie_value); 
#The setcookie() function must appear BEFORE the <html> tag.
#The value of the cookie is automatically URLencoded when sending the cookie,
# and automatically decoded when received (to prevent URLencoding, use setrawcookie() instead).


?>
<html>
    <body>
        
<?php
var_dump($_COOKIE);
print_r($_COOKIE);
// find out if the cookie is set
if(!isset($_COOKIE[$cookie_name])) {
    echo "Cookie named '" . $cookie_name . "' is not set! \n";
} else {
    echo "Cookie '" . $cookie_name . "' is set!<br> \n";
    echo "Value is: " . $_COOKIE[$cookie_name];
}
//checks whether cookies are enabled, count the $_COOKIE array variable
if(count($_COOKIE) > 0) 
{
    echo "Cookies are enabled.\n";
} 
else 
{
    echo "Cookies are disabled.\n";
}

$_SESSION[$cookie_name] = $cookie_value;
var_dump($_SESSION);
if(!isset($_SESSION[$cookie_name]))
{
    echo "Sessions are disabled.\nYou are not logged in! \n";
}
else
{
    echo "Sessions are enabled.\nYou are logged in!";
}
// remove all session variables
session_unset();

// destroy the session
session_destroy();
?>

</body>
</html>