/*************************************************************************
	> File Name:    0.两个有序数组的中位数.cpp
	> Author:       candy7 
	> Mail:         bc970321@163.com
	> Created Time: 2019年07月18日 星期四 11时10分13秒
 ************************************************************************/

#include <stdio.h>
#include <vector>
#include <limits.h>
using namespace std;

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

class Solution {
public:
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		int n = nums1.size();
		int m = nums2.size();

		if (n > m)  //保证数组1一定最短
		{
			return findMedianSortedArrays(nums2, nums1);
		}

		//Ci为第i个数组的割比如C1为2时表示第1个数组只有2个元素。
        //LMaxi为第i个数组割后的左元素。RMini为第i个数组割后的右元素。
		int LMax1, LMax2, RMin1, RMin2, c1, c2, lo = 0, hi = 2 * n;  //我们目前是虚拟加了'#'所以数组1是2*n长度

		while (lo <= hi)   //二分
		{
			c1 = (lo + hi) / 2;  //c1是二分的结果
			c2 = m + n - c1;

			LMax1 = (c1 == 0) ? INT_MIN : nums1[(c1 - 1) / 2];
			RMin1 = (c1 == 2 * n) ? INT_MAX : nums1[c1 / 2];
			LMax2 = (c2 == 0) ? INT_MIN : nums2[(c2 - 1) / 2];
			RMin2 = (c2 == 2 * m) ? INT_MAX : nums2[c2 / 2];

			if (LMax1 > RMin2)
				hi = c1 - 1;
			else if (LMax2 > RMin1)
				lo = c1 + 1;
			else
				break;
		}
		return (max(LMax1, LMax2) + min(RMin1, RMin2)) / 2.0;
	}
};


int main(int argc, char *argv[])
{
	vector<int> nums1 = { 2, 3, 5 };
	vector<int> nums2 = { 1, 4, 7, 9 };
	
	Solution solution;
	double ret = solution.findMedianSortedArrays(nums1, nums2);
    printf("%lf\n", ret);
    return 0;
}
