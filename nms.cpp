#include <iostream>
#include <opencv2/opencv.hpp>
#include <map>
using namespace cv;
using namespace std;
typedef struct
{
	Rect box;
	float confidence;
	int index;

}Bbox;
//计算IOU
static float get_iou_value(Rect rect1, Rect rect2)
{
	int xx1, yy1, xx2, yy2;
	xx1 = max(rect1.x, rect2.x);
	yy1 = max(rect1.y, rect2.y);
	xx2 = min(rect1.x + rect1.width - 1, rect2.x + rect2.width - 1);
	yy2 = min(rect1.y + rect1.height - 1, rect2.x + rect2.height - 1);
	int insection_widht, insection_height;
	insection_widht = max(0, xx2 - xx1 + 1);
	insection_height = max(0, yy2 - yy1 + 1);
	float insection_area, union_area, iou;
	insection_area = float(insection_widht * insection_height);
	union_area = float(rect1.width * rect1.height + rect2.width * rect2.height - insection_area);
	iou = insection_area / union_area;
	return iou;
}
bool comp(Bbox lsh, Bbox rsh) {
	if (lsh.confidence < rsh.confidence)
		return true;
	else
		return false;
}
/*
boxes原始检测框集合
confidences:原始检测框对应的置信度值集合
confThreshold 和 nmsThreshold分别是检测框置信度阈值以及做nms时的阈值
indices 经过上面两个阈值过滤后剩下的检测框的index
*/
void nms_boxes(vector<Rect> &boxes, vector<float> &confidences, float confThreshold, float nmsThreshold, vector<int> &indices)
{
	Bbox bbox;
	vector<Bbox> bboxes;
	int i, j;
	for (i = 0; i < boxes.size(); i++) {
		bbox.box = boxes[i];
		bbox.confidence = confidences[i];
		bbox.index = i;
		bboxes.push_back(bbox);
	}
	sort(bboxes.begin, bboxes.end, comp);
	int updated_size = bboxes.size();
	for (int i = 0; i < updated_size; i++) {
		if (bboxes[i].confidence < confThreshold)
			continue;
		indices.push_back(bboxes[i].index);
		for (j = i + 1; j < updated_size; j++) {
			float iou = get_iou_value(bboxes[i].box, bboxes[j].box);
			if (iou > nmsThreshold) {
				bboxes.erase(bboxes.begin() + j);
				updated_size = bboxes.size();
			}
		}
	}
}