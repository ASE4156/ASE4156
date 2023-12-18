./unit_test -s > reports/unit_test_report.txt
./end_test -s > reports/endtoend_test_report.txt
python3 styler_checker/run_cpplint.py . styler_checker/cpplint.py
cppcheck --enable=all --std=c++17 . --output-file=reports/static_analysis_report.txt
