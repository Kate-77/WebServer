<?php 
  $value = 'Arecookiesset'; 
  
  setcookie("TestCookie", $value); 
  setcookie("check","are cookies set") 
  ?> 
  <?php 
  // Print an individual cookie 
  // echo $_COOKIE["TestCookie"] ."\n"; 
  // echo $_COOKIE["check"] ."\n"; 
  
  // Another way to debug/test is to view all cookies 
  print_r($_COOKIE); 
?>