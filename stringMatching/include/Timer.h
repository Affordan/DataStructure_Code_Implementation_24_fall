#pragma once

/*******************************************
* Timer head file inspired by Le Zhang 2312454@mail.nankai.edu.cn
* 
*/
#include <chrono>
#include <exception>
#include <iterator>
#include <ranges>
#include <iomanip>

template <class Duration=chrono::nanoseconds>
class Timer
{
	using Clock = chrono::steady_clock;
	using TimePoint = chrono::time_point<Clock>;

private:
	Duration duration;
	TimePoint startPoint, endPoint;
public:
	Timer() :duration(Duration::zero()) {};
	~Timer() { };

	void start()
	{
		
		duration = Duration::zero();
		startPoint = Clock::now();
	};
	void stop()
	{
		endPoint = Clock::now();
		duration = chrono::duration_cast<Duration>(endPoint - startPoint);
	};
	Duration getDuration()
	{
		if (endPoint < startPoint)
			throw runtime_error("Invalid endPoint!\n");
		return duration;
	 }
};

template<class Duration=chrono::nanoseconds>
Duration add( Timer<Duration>& a,  Timer<Duration>& b)
{
	return a.getDuration() + b.getDuration();
}

template<class Duration=chrono::nanoseconds>
void formmatPrintDuration(Duration duration,const string& message="")
{
	const int WIDTH = 80;
	cout << setfill('-') << setw(WIDTH) << "\n";
	cout << setfill(' ');
	
	// 将时间转换为更易读的格式
	auto ns = chrono::duration_cast<chrono::nanoseconds>(duration).count();
	double ms = ns / 1000000.0;
	
	cout << "| " << left << setw(40) << message 
		 << "| " << right << setw(20) << fixed << setprecision(3) << ms << " ms |\n";
	
	cout << setfill('-') << setw(WIDTH) << "\n";
}

// 新增：表格式输出的辅助函数
struct TestResult {
	string charset;
	size_t patternLength;
	string position;
	int kmpComparisons;
	int bmComparisons;
	double kmpBuildTime;
	double kmpSearchTime;
	double bmBuildTime;
	double bmSearchTime;
};

void printResultTable(const vector<TestResult>& results) {
    cout << "\nResults Summary:\n";
    cout << string(120, '=') << "\n";
    
    // 打印表头
    cout << "| Charset         | Length  | Position| KMP Comp.    | BM Comp.     | KMP Build    | KMP Search   | BM Build     | BM Search    |\n";
    cout << "|                 |         |         |              |              |              |              |              |              |\n";
    cout << string(120, '-') << "\n";
    
    // 打印数据行
    for (const auto& r : results) {
        // 转换时间单位为微秒
        cout << "| " << left << setw(15) << r.charset 
             << "| " << right << setw(8) << r.patternLength 
             << "| " << setw(8) << r.position 
             << "| " << setw(12) << r.kmpComparisons
             << "| " << setw(12) << r.bmComparisons
             << "| " << setw(12) << fixed << setprecision(3) << r.kmpBuildTime
             << "| " << setw(12) << r.kmpSearchTime
             << "| " << setw(12) << r.bmBuildTime
             << "| " << setw(12) << r.bmSearchTime << "|\n";
    }
    
    cout << string(120, '=') << "\n";
    
    // 图例说明
    cout << "\nNotes:\n";
    cout << "- Time measurements in microseconds (us)\n";
    cout << "- Comp. = Number of character comparisons\n";
}

// 辅助函数：文本居中
string center(const string& text, int width) {
    int padding = width - text.length();
    int padLeft = padding / 2;
    int padRight = padding - padLeft;
    return string(padLeft, ' ') + text + string(padRight, ' ');
}