#include "web_content.h"

// Note - This a basic example code for getting the controls up and running, it may be buggy
const char *cssStyles = R"(
/* CSS styles */
body {
    font-family: Arial, sans-serif;
    background-color: #f0f0f0;
    margin: 0;
    padding: 0;
    text-align: center;
}

h1 {
    color: #333; 
    font-size: 8vw; /* Font size is 8% of viewport width */
    margin-top: 5vh; /* Margin top is 5% of viewport height */
    max-width: 90%; /* Limit the width of the heading to 90% of the viewport */
    margin-bottom: 3vh; /* Margin bottom is 3% of viewport height */
    text-align: center; /* Center align the heading */
}

a {
    color: #007BFF;
    text-decoration: none;
    font-size: 6vw; /* Font size is 6% of viewport width */
    text-align: left; /* Align file and folder names to the left */
    display: block; /* Ensure each link is displayed as a block element */
}

a:hover {
    text-decoration: underline;
}

.button-container {
    margin-top: 10vh; /* Add margin to top for separation */
    display: flex;
    flex-wrap: wrap; /* Allow flex items to wrap to the next row */
    justify-content: center;
}

.button-container button {
  flex-basis: calc(50% - 10px); /* Set the initial width of each button to 50% minus margin */
  margin: 5px; /* Add margin between buttons */
}
  
button {
    padding: 3vw 8vw; /* Padding is 3% of viewport width horizontally and 8% vertically */
    background-color: #007BFF;
    color: white;
    border: none;
    border-radius: 5px;
    cursor: pointer;
    font-size: 4vw; /* Font size is 4% of viewport width */
    margin: 0 2vw; /* Margin is 0 on top and bottom, 2% of viewport width horizontally */
}

button:hover {
    background-color: #0056b3;
}

.folder-view, .file-view {
    margin: 5vh 10vw; /* Add margin to file/folder view on top and bottom, 10% of viewport width horizontally */
}  

pre {
    font-size: 6vw; /* Font size is 6% of viewport width */
    text-align: left; /* Align file content to the left */
    margin: 5vh 10vw; /* Add margin to file contents on top and bottom, 10% of viewport width horizontally */
}

/* Add more styles as needed */
)";

const char *rootViewHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 BADUSBr</title>
    <style>%s</style>
    <script>
        function executePayloadFunction() {
            window.location.href = '/payload';
        }  
        function executeMountFunction(){
            window.location.href = '/mount_sd';
        }
        function executeClearTraceFunction() {
            window.location.href = '/clear_trace';
        }
    </script>
</head>
<body>
    <h1>Wifi-Exe</h1>
    <div class="button-container">
        <button onclick="location.href='/folder?path=/'">View Root Directory</button>
        <button onclick='executePayloadFunction()'>Execute Payload</button>
        <button onclick="location.href='/restart'">Disconnect & Restart</button>
        <button onclick='executeMountFunction()'>Mount Storage</button>
        <button onclick='executeClearTraceFunction()'>Clear Traces</button>
    </div>
</body>
</html>    
)";

const char *folderViewHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Folder View</title>
    <style>%s</style>
</head>
<body>
    <h1>Contents of folder</h1>
    <div class="folder-view">%s</div>
</body>
</html>
)";

const char *fileViewHTML = R"(
<!DOCTYPE html>
<html>
<head>
    <title>File View</title>
    <style>%s</style>
</head>
<body>
    <h1>File content</h1>
    <div class="file-view">%s</div>
    <pre>%s</pre>
    <form action="/execute_function" method="post">
        <button type="submit">Execute</button>
    </form>
</body>
</html>
)";

const char *redirectToRoot = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Executing Payload</title>
    <style>%s</style>
    <script>
        setTimeout(function() {
            window.location.href = "/";
        }, 3000); // Redirect after 3 seconds
    </script>
</head>
<body>
    <h1>Executing Payload...</h1>  
</body>
</html>
)";