#include<iostream>
#include <fstream>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;
using namespace cv; 
#define YX 1;
#define YBLJ 0
#define BNZY -1


	struct Point_to_point{
		set<int> adja_points;//��ŵ�ǰ���ڽӵ�id
		int F;//�ж�������Ƿ��Ѻ�������ƥ����߷���
	};

	struct Segment{
		Point2d pa;
		Point2d pb;
	};//����߶�

	
int cvxDirection(Point2d pi, Point2d pj, Point2d pk);// �ж�(pi->pj->pk)�ķ���
bool isSegments_Intersect(Segment s, Point2d p3,Point2d p4);
bool icvxIsOnSegment(Point2d pi, Point2d pj, Point2d pk);

int main(){
	int leds,min,group,rest;//leds��һ��ĵ�����Ŀ��min��������ȫ����ʱ���ٶ��ٸ�����һ�飬����min����Ŀ���ǹ�����group��������;rest:ʣ��һ��ĵ���
	cin>>leds;//>>min;
	Mat image(400,600,CV_8UC3,Scalar(255,255,255));
	namedWindow("Result",WINDOW_AUTOSIZE);
	int cell_n,sites_n,adjas,adja_p;
	double x1,y1,x2,y2;
	double x11,y11,x12,y12,x13,y13;
	vector<Point2d> point_intite; 
	vector<Point_to_point> adja_point_input;//��ʼ����㼯��ÿ�������Լ�����ӵ�ͷ���
	vector<Point_to_point> adja_point_deal;//�����������㼯��ÿ���㼯����Ŀ����һ���
	
	vector<vector<Segment>> vec_segment; //��Ż��ֺõ���
	Point2d pt;
	ifstream f_point;
	ifstream f_adj;
	f_point.open("E:/Ksy/power/CPD/TestReduced/10/density_CPU/final_diagram.txt",ios::in|ios::app);
	f_adj.open("E:/Ksy/power/CPD/TestReduced/10/density_CPU/adjacentPoint.txt",ios::in|ios::app);
	        f_point>>cell_n;
			for(int i=0;i<cell_n;++i){
				f_point>>x1>>x2>>y1>>y2;
				cv::Point2d start = cv::Point2d((x1)*100, (y1)*100);
                cv::Point2d end = cv::Point2d((x2)*100, (y2)*100);
				cv::line(image, start, end, cv::Scalar(0),2,CV_AA);//�߽�
			}

			f_point>>sites_n;
			for(int i=0;i<sites_n;++i){
				f_adj>>adjas;//��ӵ����Ŀ
				set<int> adja_ps;
				Point_to_point p2p;
				int flag=0;
				for(int j=0;j<adjas;++j){
					f_adj>>adja_p;
					if(!adja_point_input.empty()&&adja_p<adja_point_input.size()){
						if(adja_point_input[adja_p].F!=1)
							adja_ps.insert(adja_p);
					        p2p.adja_points=adja_ps;
					        p2p.F=YX;//������������ͼ
					}
					else{
							adja_ps.insert(adja_p);
					        p2p.adja_points=adja_ps;
					        p2p.F=YX;
					}
				}
				adja_point_input.push_back(p2p);
				//�����ڽӵ�����ͼ
				f_point>>pt.x>>pt.y;
				point_intite.push_back(pt);
			}
			
		/*	if(point_intite.size()%leds>=min||point_intite.size()%leds==0){
				 group=point_intite.size()/leds;
				 rest=point_intite.size()%leds;
			}
			else{
				 group=point_intite.size()/leds-1;
				 rest=point_intite.size()%leds+leds;
			}
*/

			int temp=point_intite.size();
				//group*leds;
			int cur_p_id=0;//��ǰ��
			vector<int> point_2_line_id;
			set<int>::iterator nex_p_id;//��ǰ�����һ����
			while(temp>0){
				vector<Point_to_point>::iterator it_pp;
				it_pp=adja_point_input.begin();
				while(it_pp!=adja_point_input.end()){
					nex_p_id=(*it_pp).adja_points.begin();
					while(nex_p_id!=(*it_pp).adja_points.end()){
						vector<int>::iterator result = find( point_2_line_id.begin( ), point_2_line_id.end( ), cur_p_id ); 
						if(result==point_2_line_id.end()){
						point_2_line_id.push_back(cur_p_id);
				    temp--;
						}
					cur_p_id=*nex_p_id;
					nex_p_id++;
						
				}
				it_pp++;
					}
				}


			Segment seg;
			vector<vector<Segment>>::iterator vec_seg_it;
			vector<Segment>::iterator seg_it;
			//for(int i=0;i<group*leds;){
			for(int i=0;i<point_intite.size();){
				vector<Point2d> point_2_line;//��Ҫ����һ��ĵ����p1,p2,p3
				int j=0;
	
				while(j<leds){
					int t=point_2_line_id[i];
					cv::Point2d p=cv::Point2d(point_intite[t].x*100,point_intite[t].y*100);
				//	cv::Point2d p=cv::Point2d(point_intite[i].x*100,point_intite[i].y*100);
					point_2_line.push_back(p);
					j++;
					i++;
				}

				vector<Segment> segment;//����߶�
				for(int id=0;id<leds-1;++id){
					 seg.pa=point_2_line[id]; 
				     seg.pb=point_2_line[id+1];
					 int intersect=0;
					 if(!vec_segment.empty()){//���
						 for(vec_seg_it=vec_segment.begin();vec_seg_it!=vec_segment.end();vec_seg_it++){
							 for(seg_it=(*vec_seg_it).begin();seg_it!=(*vec_seg_it).end();seg_it++){
							if( isSegments_Intersect(*seg_it,seg.pa,seg.pb)){
								//intersect=1;
								Point2d temp=seg.pb;
								seg.pb=(*seg_it).pb;
								(*seg_it).pb=temp;
								point_2_line[id+1]=seg.pb;
								//id--;
								vec_seg_it=vec_segment.begin();
								break;
								//seg_it=(*vec_seg_it).begin();
								 //segment.push_back(seg);	
							}//����pb��seg.pa++++++++,�Ȳ����߶Σ����߶ζ��洢������ȷ��ȫ���޽����һ���Ի� 
							else if(!isSegments_Intersect(*seg_it,seg.pa,seg.pb)&&vec_seg_it==vec_segment.end()&&seg_it==(*vec_seg_it).end())
							{
								 segment.push_back(seg);	
							}
							}
						 }
						 if(intersect==0){//������ཻ����vector segment
					                segment.push_back(seg);	
						 }
					 }
					 else
						 segment.push_back(seg);
				}
				 vec_segment.push_back(segment);
			}

		
			//for(int i=0;i<rest-1;++i){
			//	vector<Segment> segment;//����߶�
			//	int intersect=0;
			//	seg.pa=point_intite[group*leds+i];
			//	seg.pb=point_intite[group*leds+i+1];
			//		 for(vec_seg_it=vec_segment.begin();vec_seg_it!=vec_segment.end();vec_seg_it++){
			//				 for(seg_it=(*vec_seg_it).begin();seg_it!=(*vec_seg_it).end();seg_it++){
			//				if( isSegments_Intersect(*seg_it,seg.pa,seg.pb)){
			//					Point temp=seg.pb;
			//					seg.pb=(*seg_it).pb;
			//					(*seg_it).pb=temp;
			//					point_intite[group*leds+i+1]=seg.pb;
			//					//id--;
			//					vec_seg_it=vec_segment.begin();
			//					break;	
			//				}//����pb��seg.pb,�Ȳ����߶Σ����߶ζ��洢������ȷ��ȫ���޽����һ���Ի� 
			//				else if(!isSegments_Intersect(*seg_it,seg.pa,seg.pb)&&vec_seg_it==vec_segment.end()&&seg_it==(*vec_seg_it).end())
			//				{
			//					 segment.push_back(seg);	
			//				}
			//				}
			//			 }
			//			 if(intersect==0){//������ཻ����vector segment
			//		                segment.push_back(seg);	
			//			 }
			//	    /*circle(image, p1, 3,Scalar(0,0,255),-1); 
			//	    circle(image, p2, 3,Scalar(0,0,255),-1); 
			//		cv::line(image,p1,p2,cv::Scalar(0),1,CV_AA);*/
			//}

			vector<vector<Segment>>::iterator vec_seg_it2;
			vector<Segment>::iterator seg_it2;
			for(vec_seg_it2=vec_segment.begin();vec_seg_it2!=vec_segment.end();++vec_seg_it2){
				vector<Segment> segg=*vec_seg_it2;
				for(seg_it2=segg.begin();seg_it2!=segg.end();++seg_it2){
					Segment segg=*seg_it2;
					circle(image, segg.pa, 3,Scalar(0,0,255),-1);
				    circle(image, segg.pb, 3,Scalar(0,0,255),-1); 
					cv::line(image,segg.pa,segg.pb,cv::Scalar(0),1,CV_AA);
				}
			}
			imshow("Result",image);  
			waitKey(0);
}
// �ж�(pi->pj->pk)�ķ���

int cvxDirection(Point2d pi, Point2d pj, Point2d pk)
{
 int x1 = pk.x - pi.x;
 int y1 = pk.y - pi.y;

 int x2 = pj.x - pi.x;
 int y2 = pj.y - pi.y;

 return x1*y2 - x2*y1;
}

//�ж��������н��Ƿ������
bool cvxAngle(Segment s1, Segment s2){//s1(pa,pb) s2(pa,pb)
	int x1 = s1.pa.x - s1.pb.x;
	int y1 = s1.pa.y - s1.pb.y;

	int x2 = s2.pb.x - s2.pa.x;
	int y2 = s2.pb.y - s2.pa.y;

 if((x1*x2 + y1*y2)<=0)
	 return true;
 else return false;
}
//�ж��ĸ�������ģ���󣬽�С��ģ��
bool isAlongerthanB(Segment s1, Segment s2){
	int x1=s1.pa.x-s1.pb.x;
	int y1=s1.pa.y-s1.pb.y;

	int x2=s2.pa.x-s2.pb.x;
	int y2=s2.pa.y-s2.pb.y;

	if((x1*x1+y1*y1)>(x2*x2+y2*y2))
		return true; //s1>s2,��
	else return false;//s1<s2;
}

bool isSegments_Intersect(Segment s, Point2d p3,Point2d p4){
	Point2d p1=s.pa;Point2d p2=s.pb;
	int d1=cvxDirection(p3,p4,p1);
	int d2=cvxDirection(p3,p4,p2);
	int d3=cvxDirection(p1,p2,p3);
	int d4=cvxDirection(p1,p2,p4);
	if(((d1>0&&d2<0)||(d1<0&&d2>0))&&((d3>0&&d4<0)||(d3<0&&d4>0)))
		return true;
	

	 // �߶�ĳ���˵�λ����һ���߶���

 if(d1 == 0 && icvxIsOnSegment(p3, p4, p1))
  return true;
 if(d2 == 0 && icvxIsOnSegment(p3, p4, p2))
  return true;
 if(d3 == 0 && icvxIsOnSegment(p1, p2, p3))
  return true;
 if(d4 == 0 && icvxIsOnSegment(p1, p2, p4))
  return true;

 // ���ཻ

 return false;
}

bool icvxIsOnSegment(Point2d pi, Point2d pj, Point2d pk)
{
 int minx = (pi.x < pj.x)? pi.x: pj.x;
 int maxx = (pi.x > pj.x)? pi.x: pj.x;

 int miny = (pi.y < pj.y)? pi.y: pj.y;
 int maxy = (pi.y > pj.y)? pi.y: pj.y;

 if( pk.x >= minx && pk.x <= maxx
  && pk.y >= miny && pk.y <= maxy)
  return true;
  else return false;
}                                                                                                                                             