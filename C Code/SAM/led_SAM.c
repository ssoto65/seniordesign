#include "led_SAM.h"
#include "sam.h"

int array_addressing[32][32] = {{961,963,965,967,969,971,973,975,977,979,981,983,985,987,989,991,993,995,997,999,1001,1003,1005,1007,1009,1011,1013,1015,1017,1019,1021,1023},
{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59,61,63},
{65,67,69,71,73,75,77,79,81,83,85,87,89,91,93,95,97,99,101,103,105,107,109,111,113,115,117,119,121,123,125,127},
{129,131,133,135,137,139,141,143,145,147,149,151,153,155,157,159,161,163,165,167,169,171,173,175,177,179,181,183,185,187,189,191},
{193,195,197,199,201,203,205,207,209,211,213,215,217,219,221,223,225,227,229,231,233,235,237,239,241,243,245,247,249,251,253,255},
{257,259,261,263,265,267,269,271,273,275,277,279,281,283,285,287,289,291,293,295,297,299,301,303,305,307,309,311,313,315,317,319},
{321,323,325,327,329,331,333,335,337,339,341,343,345,347,349,351,353,355,357,359,361,363,365,367,369,371,373,375,377,379,381,383},
{385,387,389,391,393,395,397,399,401,403,405,407,409,411,413,415,417,419,421,423,425,427,429,431,433,435,437,439,441,443,445,447},
{449,451,453,455,457,459,461,463,465,467,469,471,473,475,477,479,481,483,485,487,489,491,493,495,497,499,501,503,505,507,509,511},
{513,515,517,519,521,523,525,527,529,531,533,535,537,539,541,543,545,547,549,551,553,555,557,559,561,563,565,567,569,571,573,575},
{577,579,581,583,585,587,589,591,593,595,597,599,601,603,605,607,609,611,613,615,617,619,621,623,625,627,629,631,633,635,637,639},
{641,643,645,647,649,651,653,655,657,659,661,663,665,667,669,671,673,675,677,679,681,683,685,687,689,691,693,695,697,699,701,703},
{705,707,709,711,713,715,717,719,721,723,725,727,729,731,733,735,737,739,741,743,745,747,749,751,753,755,757,759,761,763,765,767},
{769,771,773,775,777,779,781,783,785,787,789,791,793,795,797,799,801,803,805,807,809,811,813,815,817,819,821,823,825,827,829,831},
{833,835,837,839,841,843,845,847,849,851,853,855,857,859,861,863,865,867,869,871,873,875,877,879,881,883,885,887,889,891,893,895},
{897,899,901,903,905,907,909,911,913,915,917,919,921,923,925,927,929,931,933,935,937,939,941,943,945,947,949,951,953,955,957,959},
{960,962,964,966,968,970,972,974,976,978,980,982,984,986,988,990,992,994,996,998,1000,1002,1004,1006,1008,1010,1012,1014,1016,1018,1020,1022},
{0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62},
{64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126},
{128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190},
{192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,254},
{256,258,260,262,264,266,268,270,272,274,276,278,280,282,284,286,288,290,292,294,296,298,300,302,304,306,308,310,312,314,316,318},
{320,322,324,326,328,330,332,334,336,338,340,342,344,346,348,350,352,354,356,358,360,362,364,366,368,370,372,374,376,378,380,382},
{384,386,388,390,392,394,396,398,400,402,404,406,408,410,412,414,416,418,420,422,424,426,428,430,432,434,436,438,440,442,444,446},
{448,450,452,454,456,458,460,462,464,466,468,470,472,474,476,478,480,482,484,486,488,490,492,494,496,498,500,502,504,506,508,510},
{512,514,516,518,520,522,524,526,528,530,532,534,536,538,540,542,544,546,548,550,552,554,556,558,560,562,564,566,568,570,572,574},
{576,578,580,582,584,586,588,590,592,594,596,598,600,602,604,606,608,610,612,614,616,618,620,622,624,626,628,630,632,634,636,638},
{640,642,644,646,648,650,652,654,656,658,660,662,664,666,668,670,672,674,676,678,680,682,684,686,688,690,692,694,696,698,700,702},
{704,706,708,710,712,714,716,718,720,722,724,726,728,730,732,734,736,738,740,742,744,746,748,750,752,754,756,758,760,762,764,766},
{768,770,772,774,776,778,780,782,784,786,788,790,792,794,796,798,800,802,804,806,808,810,812,814,816,818,820,822,824,826,828,830},
{832,834,836,838,840,842,844,846,848,850,852,854,856,858,860,862,864,866,868,870,872,874,876,878,880,882,884,886,888,890,892,894},
{896,898,900,902,904,906,908,910,912,914,916,918,920,922,924,926,928,930,932,934,936,938,940,942,944,946,948,950,952,954,956,958}};
	
	
int win_screen[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,1,0,0,1,1,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

int lose_screen[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

int bam_option[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,0,0,1,1,0,0,0,0,1},
{1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,1},
{1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,1},
{1,0,0,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,2,2,0,0,2,2,0,0,2,2,2,0,0,2,2,0,2,0,0,2,0,0,0,0,0,1},
{1,0,0,0,0,0,2,0,2,0,2,0,2,0,0,2,0,0,2,0,0,0,2,0,2,0,0,0,0,0,0,1},
{1,0,0,0,0,0,2,2,0,0,2,2,0,0,0,2,0,0,2,0,0,0,2,2,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,2,0,2,0,2,0,2,0,0,2,0,0,2,0,0,0,2,0,2,0,0,0,0,0,0,1},
{1,0,0,0,0,0,2,2,0,0,2,0,2,0,2,2,2,0,0,2,2,0,2,0,0,2,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,1,0,0,0,3,0,0,0,3,0,0,3,3,0,0,3,3,3,3,3,0,3,3,3,0,0,0,0,0,1},
{1,0,0,1,0,0,3,3,0,3,3,0,3,0,0,3,0,0,0,0,3,0,0,3,0,0,0,0,0,0,0,1},
{1,0,0,0,1,0,3,0,3,0,3,0,3,3,3,3,0,0,0,3,0,0,0,3,3,3,0,0,0,0,0,1},
{1,0,0,1,0,0,3,0,0,0,3,0,3,0,0,3,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,1},
{1,0,1,0,0,0,3,0,0,0,3,0,3,0,0,3,0,3,3,3,3,3,0,3,3,3,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

int bb_option[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,0,0,1,1,0,0,0,0,1},
{1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,1},
{1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,1},
{1,0,0,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,1,0,0,0,2,2,0,0,2,2,0,0,2,2,2,0,0,2,2,0,2,0,0,2,0,0,0,0,0,1},
{1,0,0,1,0,0,2,0,2,0,2,0,2,0,0,2,0,0,2,0,0,0,2,0,2,0,0,0,0,0,0,1},
{1,0,0,0,1,0,2,2,0,0,2,2,0,0,0,2,0,0,2,0,0,0,2,2,0,0,0,0,0,0,0,1},
{1,0,0,1,0,0,2,0,2,0,2,0,2,0,0,2,0,0,2,0,0,0,2,0,2,0,0,0,0,0,0,1},
{1,0,1,0,0,0,2,2,0,0,2,0,2,0,2,2,2,0,0,2,2,0,2,0,0,2,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,3,0,0,0,3,0,0,3,3,0,0,3,3,3,3,3,0,3,3,3,0,0,0,0,0,1},
{1,0,0,0,0,0,3,3,0,3,3,0,3,0,0,3,0,0,0,0,3,0,0,3,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,3,0,3,0,3,0,3,3,3,3,0,0,0,3,0,0,0,3,3,3,0,0,0,0,0,1},
{1,0,0,0,0,0,3,0,0,0,3,0,3,0,0,3,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,3,0,0,0,3,0,3,0,0,3,0,3,3,3,3,3,0,3,3,3,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};



void set_LED(int row, int column, unsigned long data){
	volatile unsigned long address = array_addressing[row][column];
	
	volatile uint32_t *address_start =  (uint32_t *)0x60000000;
	
	for(int ii=0; ii<50;ii++){
	asm("nop");}

	address_start[address] = data;
	
	for(int ii=0; ii<50;ii++){
		asm("nop");}
	
}

void clear_matrix(void){
	for (uint8_t i = 0; i<=31; i++){
		for(uint8_t j = 0; j<=31; j++){
			set_LED(i,j,0);
		}
	}
} 

void ebi_init(void){
	
	REG_PMC_PCER0 |= PMC_PCER0_PID10 + PMC_PCER0_PID11 + PMC_PCER0_PID12 + PMC_PCER0_PID13;
	//Disable Pins from PDIO
	
	//REG_PIOC_WPMR = PIO_WPMR_WPKEY(PIO_WPMR_WPKEY_PASSWD);
	
	//REG_PIOC_PDR = 0xFFFFFFFF;
	
	REG_PIOC_PDR = PIO_PER_P18 |  PIO_PER_P19 | PIO_PER_P20 | PIO_PER_P21 | PIO_PER_P22 | PIO_PER_P23 | PIO_PER_P24 | PIO_PER_P25 |
					 PIO_PER_P26 | PIO_PER_P27 | PIO_PER_P28 | PIO_PER_P29 | PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P0 | PIO_PER_P1 |
					  PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8 | PIO_PER_P9 |
						 PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P10 | PIO_PER_P11 | PIO_PER_P13 | PIO_PER_P14;
		
	//REG_PIOC_PER = 0xFFFFFFFF;
	
	
	//Enable Peripheral	
		 
	REG_PIOC_ABCDSR &=  ~(PIO_PER_P18 |  PIO_PER_P19 | PIO_PER_P20 | PIO_PER_P21 | PIO_PER_P22 | PIO_PER_P23 | PIO_PER_P24 | PIO_PER_P25 |
							PIO_PER_P26 | PIO_PER_P27 | PIO_PER_P28 | PIO_PER_P29 | PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P0 | PIO_PER_P1 |
								PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4 | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8 |  PIO_PER_P11 | 
									PIO_PER_P30 | PIO_PER_P31 | PIO_PER_P14 | PIO_PER_P9 | PIO_PER_P13 | PIO_PER_P10 );
								
								
	//CCFG REgister?
	REG_CCFG_SMCNFCS = CCFG_SMCNFCS_SMC_NFCS0;
	
	//Disabling Write Protection Register
	REG_SMC_WPMR = SMC_WPMR_WPKEY_PASSWD;
	
	//SMC Setup Length
	REG_SMC_SETUP0  = SMC_SETUP_NCS_RD_SETUP(1) + SMC_SETUP_NRD_SETUP(1) + SMC_SETUP_NCS_WR_SETUP(1) + SMC_SETUP_NWE_SETUP(1);
	
	//SMC Pulse Length
	REG_SMC_PULSE0 = SMC_PULSE_NCS_RD_PULSE(2) + SMC_PULSE_NRD_PULSE(2) + SMC_PULSE_NCS_WR_PULSE(2) + SMC_PULSE_NWE_PULSE(1);
	
	//SMC Cycle Length 
	REG_SMC_CYCLE0 = SMC_CYCLE_NRD_CYCLE(4) + SMC_CYCLE_NWE_CYCLE(4);
	
	REG_SMC_MODE0 = SMC_MODE_PS(SMC_MODE_PS_8_BYTE) + SMC_MODE_PMEN;
	
	//SMC Mode
	//REG_SMC_MODE0
	/*
	//Enabling Write Protection
	REG_SMC_WPMR = SMC_WPMR_WPKEY_PASSWD + SMC_WPMR_WPEN;
	
	*/
	
	clk_out();
	
}

void clk_out(void){
	
	//REG_CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCF(CKGR_MOR_MOSCRCF_12_MHz);
	
	REG_PIOA_PDR = PIO_PER_P6;
	
	REG_PIOA_ABCDSR = PIO_PER_P6;
	
	REG_PMC_PCK |= PMC_PCK_CSS(PMC_PCK_CSS_MCK);
	
	REG_PMC_SCER |= PMC_SCER_PCK0;
	
}

void display_win(void){
	for(int ii = 0; ii<32; ii++){
		for(int jj = 0; jj<32; jj++){
			if(win_screen[ii][jj]== 1){
				set_LED(ii,jj,0x00FF00);
			}
			else{
				set_LED(ii,jj,0);
			}
		}
	}
}

void display_lose(void){
	for(int ii = 0; ii<32; ii++){
		for(int jj = 0; jj<32; jj++){
			if(lose_screen[ii][jj]== 1){
				set_LED(ii,jj,0xFF);
			}
			else{
				set_LED(ii,jj,0);
			}
		}
	}
}

void display_menu_bam(void){
		for(int ii = 0; ii<32; ii++){
			for(int jj = 0; jj<32; jj++){
				if(bam_option[ii][jj]== 1){
					set_LED(ii,jj,0xFFFFFF);
				}
				else if(bam_option[ii][jj]== 2){
					set_LED(ii,jj,0xFF);
				}
				else if(bam_option[ii][jj]== 3){
					set_LED(ii,jj,0x00FF00);
				}
				else{
					set_LED(ii,jj,0);
				}
			}
		}
	
}

void display_menu_bb(void){
	for(int ii = 0; ii<32; ii++){
		for(int jj = 0; jj<32; jj++){
			if(bb_option[ii][jj]== 1){
				set_LED(ii,jj,0xFFFFFF);
			}
			else if(bb_option[ii][jj]== 3){
				set_LED(ii,jj,0xFF);
			}
			else if(bb_option[ii][jj]== 2){
				set_LED(ii,jj,0x00FF00);
			}
			else{
				set_LED(ii,jj,0);
			}
		}
	}
	
}