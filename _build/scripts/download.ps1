# Force errors to bubble correctly.
trap { exit 1 }

# Download the file (using WebClient to support Windows 7.)
$WebClient = New-Object System.Net.WebClient
$WebClient.DownloadFile($args[0], $args[1])
