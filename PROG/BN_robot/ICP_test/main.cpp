#include <iostream>
#include <vector>
#include "../utility.h"
#include "ICP.h"

using namespace gs;

void createPoints0(std::vector<Point*>& points) {
	std::vector<std::vector<double>> a = Utility::readCSV_DOUBLE("graphXY0.csv",";");
	for(unsigned int i=0;i<a.at(0).size();i++) {
        if(!isinf(a.at(0).at(i))) {
            points.push_back(new Point(static_cast<float>(a.at(0).at(i)), static_cast<float>(a.at(1).at(i)), 0.0f));
            std::cout << static_cast<float>(a.at(0).at(i)) << " " << static_cast<float>(a.at(1).at(i)) << std::endl;
        } else {
            points.push_back(new Point(0.0f, 0.0f, 0.0f));
            std::cout << "0.0f 0.0f" << std::endl;
        }
	}
}

void createPoints1(std::vector<Point*>& points) {
    std::vector<std::vector<double>> a = Utility::readCSV_DOUBLE("graphXY0.csv",";");
	for(unsigned int i=0;i<a.at(0).size();i++) {
        if(!isinf(a.at(0).at(i))) {
            points.push_back(new Point(static_cast<float>(a.at(0).at(i)), static_cast<float>(a.at(1).at(i)), 0.0f));
            std::cout << static_cast<float>(a.at(0).at(i)) << " " << static_cast<float>(a.at(1).at(i)) << std::endl;
        } else {
            points.push_back(new Point(0.0f, 0.0f, 0.0f));
            std::cout << "0.0f 0.0f" << std::endl;
        }
	}
}

void applyAffineTransform(std::vector<Point*>& points, float* rotationMatrix, float* translation) {
	Point pRot;
	for (int i = 0; i < points.size(); i++)
	{
		pRot.pos[0] = rotationMatrix[0] * points[i]->pos[0] + rotationMatrix[1] * points[i]->pos[1] + rotationMatrix[2] * points[i]->pos[2] + translation[0];
		pRot.pos[1] = rotationMatrix[3] * points[i]->pos[0] + rotationMatrix[4] * points[i]->pos[1] + rotationMatrix[5] * points[i]->pos[2] + translation[1];
		pRot.pos[2] = rotationMatrix[6] * points[i]->pos[0] + rotationMatrix[7] * points[i]->pos[1] + rotationMatrix[8] * points[i]->pos[2] + translation[2];

		*points[i] = pRot;
	}
}

int main() {
	std::vector<Point*> staticPointCloud;
	createPoints0(staticPointCloud);
	std::vector<Point*> dynamicPointCloud;
	createPoints1(dynamicPointCloud);

	float rotation[] = { 1.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 1.0f };
	float translation[] = { 0.5f, 0.0f, 0.0f };
	applyAffineTransform(dynamicPointCloud, rotation, translation);

	icp(dynamicPointCloud, staticPointCloud);

	for(unsigned int i=0;i<dynamicPointCloud.size();i++) {
        std::cout << dynamicPointCloud[i]->pos[0] << " " << dynamicPointCloud[i]->pos[1] << " " << dynamicPointCloud[i]->pos[2] << std::endl;
	}

	float alignmentError = 0.0f;
	for (int i = 0; i < dynamicPointCloud.size(); i++)
	{
		alignmentError += pow(dynamicPointCloud[i]->pos[0] - staticPointCloud[i]->pos[0], 2.0f);
		alignmentError += pow(dynamicPointCloud[i]->pos[1] - staticPointCloud[i]->pos[1], 2.0f);
		alignmentError += pow(dynamicPointCloud[i]->pos[2] - staticPointCloud[i]->pos[2], 2.0f);
	}

	alignmentError /= (float)dynamicPointCloud.size();

	printf("Alignment Error: %0.5f \n", alignmentError);

    return 0;
}
