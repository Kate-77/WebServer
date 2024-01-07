<?php
// Check if the form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    
    // Check if the file was uploaded without errors
    if (isset($_FILES["uploadedFile"]) && $_FILES["uploadedFile"]["error"] == UPLOAD_ERR_OK) {
        
        // Specify the directory where you want to save the uploaded file
        $targetDirectory = "uploads/";
        
        // Get the name of the uploaded file
        $fileName = basename($_FILES["uploadedFile"]["name"]);
        
        // Create the full path to save the file
        $targetFilePath = $targetDirectory . $fileName;
        
        // Move the uploaded file to the destination directory
        if (move_uploaded_file($_FILES["uploadedFile"]["tmp_name"], $targetFilePath)) {
            echo "File has been uploaded successfully.";
        } else {
            echo "Sorry, there was an error uploading your file.";
        }
    } else {
        echo "Error: " . $_FILES["uploadedFile"]["error"];
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>File Upload Form</title>
</head>
<body>

    <form action="<?php echo $_SERVER["PHP_SELF"]; ?>" method="post" enctype="multipart/form-data">
        <label for="uploadedFile">Choose a file:</label>
        <input type="file" name="uploadedFile" id="uploadedFile">
        <button type="submit" name="submit">Upload</button>
    </form>

</body>
</html>
