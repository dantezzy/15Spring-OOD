
echo Demonstrating requirement 7 - Show Summary

Debug\Executive.exe ./ *.h *.*

echo Demonstrating requirement 3 - Search Entire Directories /s

Debug\Executive.exe ./ *.h *.* /s

echo Demonstrating requirement 5 - Show Duplicate Files /d

Debug\Executive.exe ./ *.h *.* /s /d

echo Demonstrating requirement 6 - Show Searching Text /f

Debug\Executive.exe ./ *.h *.* /s /f"ZzY LiKe C++"

echo Demonstrating all requirements

Debug\Executive.exe ./ *.h *.* /d /f"ZzY LiKe C++" /s