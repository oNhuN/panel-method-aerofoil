g++ -std=c++20 -Wall -Wextra -Werror -g main.cpp naca_geometry.cpp write_csv.cpp panel.cpp solver.cpp linear_solve.cpp -o main.exe
if ($LASTEXITCODE -ne 0) { exit 1 }

g++ -std=c++20 -Wall -Wextra -Werror -g validate.cpp naca_geometry.cpp panel.cpp solver.cpp linear_solve.cpp -o validate.exe
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "Build succeeded."