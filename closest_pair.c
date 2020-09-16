#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

struct R2_point
{
	double x;
	double y;
};


double distance (struct R2_point *A, struct R2_point *B);
double distance (struct R2_point *A, struct R2_point *B)
{
	return sqrt((A->x - B->x)*(A->x - B->x) + (A->y - B->y)*(A->y - B->y));
}

void swap (struct R2_point *A, struct R2_point *B);
void swap (struct R2_point *A, struct R2_point *B)
{
	struct R2_point *buf;
	buf = (struct R2_point *)malloc(sizeof(struct R2_point));
	buf->x = A->x;
	buf->y = A->y;
	A->x = B->x;
	A->y = B->y;
	B->x = buf->x;
	B->y = buf->y;
	free(buf);
}

void heapify( struct R2_point *arr , int heapsize, int i);
void heapify( struct R2_point *arr , int heapsize, int i)
{ 
  int l = 2*i+1, r = 2*i+2, largest = 0;
  double buf=0;
  if ((l <= heapsize - 1) && (arr[l].y < arr[i].y))
    largest = l;
  else
    largest = i;
  if ((r <= heapsize - 1) && (arr[r].y < arr[largest].y))
    largest = r;
  if (largest != i)
   {
     buf = arr[i].y;
     arr[i].y = arr[largest].y;
     arr[largest].y = buf;
     heapify(arr,heapsize,largest);
   }
}

void makeheap(struct R2_point *arr , int len, int heapsize);
void makeheap(struct R2_point *arr , int len, int heapsize)
{
  int i;
  for(i = (len / 2)-1 ; i >= 0; i--)
    heapify(arr,heapsize,i);
}
  
  
  


void heapst(struct R2_point *arr , int len);
void heapst(struct R2_point *arr , int len)
{
  int i, heapsize=len;
  double buf = 0;
  makeheap(arr, len, heapsize);
  for (i = len - 1; i >= 0; i --)
  {
    buf = arr[0].y;
    arr[0].y = arr[i].y;
    arr[i].y = buf;
    heapsize = heapsize - 1;
    heapify(arr,heapsize,0);
  }
}

double closest_pair_plane (struct R2_point *points, int N);
double closest_pair_plane (struct R2_point *points, int N)
{
	double median, left_dist, right_dist, min, buf_dist, buf_y_dist;
	int count = 0 , count_left_closer_than_min = 0, count_right_closer_than_min = 0, upper_bound;
	int check = 0;
	struct R2_point * left_points, * right_points, * left_closer_than_min_points, * right_closer_than_min_points;

	if (N == 1)
		return INFINITY;
	else if (N == 2)
		return distance (points, points + 1);

	//preventing from seg fault
	for (int q = 0; q < N -1; q ++)
	{
		if (fabs(points[q].x - points[q+1].x) > 1e-6)
		{
			check = 1;
			break;
		}
	}
	if (!check)
		points[0].x += 2e-6;

	//finding x-median
	median = 0;
	for (int i = 0; i < N; i ++)
	{	
		printf("adding a point to median\n");
		median += points[i].x;
	}
	median /= N;
	printf("%lf\n", median);

	//determinig points, that are to the left and to right to median
	for (int i = 0; i < N; i ++)
	{

		if(points[i].x <= median)
		{
			if(i != count)
			{
				swap(points + i, points + count);
				count ++;
			}
			else
				count ++;
		}
		//printf("hi\n");
	}

	left_points = points;
	right_points = points + count;

	//recurcisvly
	left_dist = closest_pair_plane(left_points, count);
	right_dist = closest_pair_plane(right_points, N - count);

	min = min(left_dist, right_dist);

	//determening points in left_points that are in min distance to the median line
	for (int i = 0; i < count; i ++)
	{

		if(median - left_points[i].x <= min)
		{
			if(i != count_left_closer_than_min)
			{
				swap(&left_points[i], &left_points[count_left_closer_than_min]);
				count_left_closer_than_min ++;
			}
			else
				count_left_closer_than_min ++;
		}
		//printf("hi\n");
	}

	//determening points in right_points that are in min distance to the median line
	for (int i = 0; i < N - count; i ++)
	{

		if(right_points[i].x - median <= min)
		{
			if(i != count_right_closer_than_min)
			{
				swap(&right_points[i], &right_points[count_right_closer_than_min]);
				count_right_closer_than_min ++;
			}
			else
				count_right_closer_than_min ++;
		}
		//printf("hi\n");
	}

	left_closer_than_min_points = left_points;
	right_closer_than_min_points = right_points;


	//sort by y-coordinate
	heapst(left_closer_than_min_points, count_left_closer_than_min);
	heapst(right_closer_than_min_points, count_right_closer_than_min);

	//for each point in left_closer determine max 6 min_neibours and check the dist
	upper_bound = 0;
	for (int k = 0; k < count_left_closer_than_min; k ++)
	{

		for (int s = upper_bound; s < count_right_closer_than_min; s ++)
		{
			if (fabs(left_closer_than_min_points[k].y-right_closer_than_min_points[s].y) < min)
			{
				buf_dist = distance(left_closer_than_min_points + k, &right_closer_than_min_points[s]);
				if (buf_dist < min)
					min = buf_dist;
				upper_bound ++;
				s ++;
				for (int r = s; r < min (s + 6, count_right_closer_than_min); s ++)
				{
					

					buf_y_dist = fabs(left_closer_than_min_points[k].y-right_closer_than_min_points[r].y);
					

					if (buf_y_dist < min  )
					{
						buf_dist = distance(left_closer_than_min_points + k , &right_closer_than_min_points[r]);
						if (buf_dist < min)
							min = buf_dist;
						upper_bound ++;
					}
					upper_bound ++;
					break;
				}
				break;
			}
			upper_bound ++;
		}
		if (upper_bound > count_right_closer_than_min)
			break;
	}
	
	return min;

 
}
 

 int main(void)
 {
 	struct R2_point *points;
 	int N = 7;
 	double distance;

 	points = (struct R2_point *)malloc(N * sizeof(struct R2_point));
 	points[0].x = 0;
 	points[0].y = 0;
 	points[1].x = 0;
 	points[1].y = 0.3;
 	points[2].x = 0;
 	points[2].y = 0.5;
	points[3].x = 3;
 	points[3].y = 4;
 	points[4].x = 1;
 	points[4].y = 1;
 	points[5].x = 3;
 	points[5].y = 2;
 	points[6].x = 2;
 	points[6].y = 1;

 	/*if (INFINITY > 5)
 		printf("greater\n");

 	heapst(points, N);
 	for (int j = 0; j < N; j ++)
	{
		printf("%lf\n", points[j].y);
	}*/

 	printf("result = %lf\n", closest_pair_plane(points, N));
 	return 1;
 }