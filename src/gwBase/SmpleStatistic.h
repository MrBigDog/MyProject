#ifndef GWBASE_SMPLESTATISTIC_H
#define GWBASE_SMPLESTATISTIC_H 1
/************************************************************************/
/* 样本统计相关;                                                         */
/************************************************************************/
#include <compiler.h>
#include <gwBase/Export.h>

#undef min
#undef max

namespace gwBase
{
	class GW_BASE_EXPORT SampleStatistic
	{
	protected:
		int n;
		double x;
		double x2;
		double minValue, maxValue;
		double totalTime, cumulativeTime;

	public:
		SampleStatistic();
		inline virtual ~SampleStatistic();
		virtual void reset();

		virtual void operator += (double);
		int samples() const;
		double mean() const;
		double stdDev() const;
		double var() const;
		double min() const;
		double max() const;
		double total() const;
		double cumulative() const;
		double confidence(int p_percentage) const;
		double confidence(double p_value) const;

		void error(const char *msg);
	};

	inline SampleStatistic::SampleStatistic()
	{
		cumulativeTime = 0;
		reset();
	}

	inline int SampleStatistic::samples() const
	{
		return (n);
	}

	inline double SampleStatistic::min() const
	{
		return (minValue);
	}

	inline double SampleStatistic::max() const
	{
		return (maxValue);
	}

	inline double SampleStatistic::total() const
	{
		return (totalTime);
	}

	inline double SampleStatistic::cumulative() const
	{
		return (cumulativeTime);
	}

	inline SampleStatistic::~SampleStatistic()
	{
	}
}

#endif // GWUTIL_SMPLESTATISTIC_H
