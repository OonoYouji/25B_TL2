# Start-Process -FilePath TextureConverter.exe -ArgumentList uvChecker.png -Wait
$files = Get-Item *.jpg

foreach ($f in $files) {
    Start-Process -FilePath TextureConverter.exe -ArgumentList $f.FullName -Wait
}

pause