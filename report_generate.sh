./unit_test -s > reports/unit_test_report.txt
./end_test -s > reports/EndtoEnd_test_report.txt

python3 styler_checker/run_cpplint.py . styler_checker/cpplint.py