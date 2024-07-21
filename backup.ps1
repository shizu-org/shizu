# Create an archive file shizu-yyyy-MM-dd.zip and its checksum file shizu-yyyy-MM-dd.md5
# Fail if one of the files already exists.

# Backup all files except for Git specific files.
$Files = Get-ChildItem -Path . -Exclude '.gitignore', '.git'
#Write-Host 'files to backup:' + $Files

############################################################
$date = $(get-date -f yyyy-MM-dd)
$TargetFileName = "shizu-$date"
if (Test-Path ('./' + $TargetFileName + '.zip')) {
  throw "file $TargetFileName.zip already exists";
}
if (Test-Path ('./' + $TargetFileName + '.sha256')) {
  throw "file $TargetFileName.sha256 already exists";
}
if (Test-Path ('./' + $TargetFileName + '.sha1')) {
  throw "file $TargetFileName.sha1 already exists";
}
if (Test-Path ('./' + $TargetFileName + '.md5')) {
  throw "file $TargetFileName.md5 already exists";
}

############################################################
$7zipPath = "$env:ProgramFiles\7-Zip\7z.exe"
if (-not (Test-Path -Path $7zipPath -PathType Leaf)) {
  throw "7 zip file '$7zipPath' not found"
}

Set-Alias Start-SevenZip $7zipPath
Start-SevenZip a -mx=9 ('./' + $TargetFileName + '.zip') $Files

############################################################
$ExpectedHash = Get-FileHash ('./' + $TargetFileName + '.zip') -Algorithm SHA256
#Write-Host 'contents to write:' + $ExpectedHash.Hash + ' ' + ($TargetFileName + '.zip')
Set-Content -Path ('./' + $TargetFileName + '.sha256') -Value ($ExpectedHash.Hash + ' ' + $TargetFileName + '.zip')

$ExpectedHash = Get-FileHash ('./' + $TargetFileName + '.zip') -Algorithm SHA1
#Write-Host 'contents to write:' + $ExpectedHash.Hash + ' ' + ($TargetFileName + '.zip')
Set-Content -Path ('./' + $TargetFileName + '.sha1')   -Value ($ExpectedHash.Hash + ' ' + $TargetFileName + '.zip')

$ExpectedHash = Get-FileHash ('./' + $TargetFileName + '.zip') -Algorithm MD5
#Write-Host 'contents to write:' + $ExpectedHash.Hash + ' ' + ($TargetFileName + '.zip')
Set-Content -Path ('./' + $TargetFileName + '.md5')    -Value ($ExpectedHash.Hash + ' ' + $TargetFileName + '.zip')
