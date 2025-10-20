# --------------- 実行時の引数チェック ---------------
if ($args.Count -eq 0) {
    Write-Host "使用方法: ./convert.ps1 <MipLevel>"
    exit
}

$mlLevelInput = $args[0]

# 数字かどうかチェック（0も正しく判定される）
[int]$mlLevel = 0
if (-not [int]::TryParse($mlLevelInput, [ref]$mlLevel)) {
    Write-Host "数字を入力してください。処理を終了します。"
    exit
}

# MipLevelの範囲チェック（必要に応じて調整）
if ($mlLevel -lt 0 -or $mlLevel -gt 10) {
    Write-Host "MipLevelは0～10の範囲で指定してください。"
    exit
}

# --------------- 画像取得 ---------------
$files = Get-Item *.jpg
if ($files.Count -eq 0) {
    Write-Host "対象画像が見つかりませんでした。"
    exit
}

# --------------- TextureConverter 実行 ---------------
foreach ($f in $files) {
    Start-Process -FilePath "TextureConverter.exe" -ArgumentList "$($f.FullName) -ml $mlLevel" -Wait
}

Write-Host "すべての画像処理が完了しました。"
pause
