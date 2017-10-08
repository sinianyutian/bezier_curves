#include "Bezier.h"
using namespace bezier_curves;
#include <math.h>

Bezier::Bezier(PointF b, int n, System::Collections::Generic::List<GPoint^>^ p)
{
	this->b = b;
	this->p = p;
	this->n = n;
}


int bezier_curves::Bezier::factorial(int N)
{
	if (N < 0)
		return 0;
	if (N == 0)
		return 1;
	else
		return N * factorial(N - 1);
}

float *bezier_curves::Bezier::deCasteljau(float **points, float t) {
	float *pointsQ = new float[(n + 1) * 3];
	for (int j = 0; j <= n; j++) {
		pointsQ[j * 3 + 0] = points[j][0];
		pointsQ[j * 3 + 1] = points[j][1];
		pointsQ[j * 3 + 2] = points[j][2];
	}
	for (int k = 1; k <= n; k++) {
		for (int j = 0; j <= n - k; j++) {
			pointsQ[j * 3 + 0] = (1 - t) * pointsQ[j * 3 + 0] + t * pointsQ[(j + 1) * 3 + 0];
			pointsQ[j * 3 + 1] = (1 - t) * pointsQ[j * 3 + 1] + t * pointsQ[(j + 1) * 3 + 1];
			pointsQ[j * 3 + 2] = (1 - t) * pointsQ[j * 3 + 2] + t * pointsQ[(j + 1) * 3 + 2];
		}
	}
	float *result = new float[3];
	result[0] = pointsQ[0];
	result[1] = pointsQ[1];
	result[2] = pointsQ[2];
	delete[] pointsQ;
	return result;
}

float bezier_curves::Bezier::get_x_third(float t, int index)
{
	return pow((1 - t), 3) * p[index - 4]->getPoint()->X + 3 * t * pow((1 - t), 2) * p[index - 3]->getPoint()->X + 3 * t * t * (1 - t) * p[index - 2]->getPoint()->X + t * t * t * p[index - 1]->getPoint()->X;
}

float bezier_curves::Bezier::get_y_third(float t, int index)
{
	return pow((1 - t), 3) * p[index - 4]->getPoint()->Y + 3 * t * pow((1 - t), 2) * p[index - 3]->getPoint()->Y + 3 * t * t * (1 - t) * p[index - 2]->getPoint()->Y + t * t * t * p[index - 1]->getPoint()->Y;
}

float bezier_curves::Bezier::get_x_arbitrary(float t)
{
	float x = 0.0f;
	for (int i = 0; i < n; i++)
	{
		x = x + p[i]->getPoint()->X * (factorial(n) / (factorial(i) * factorial(n - i))) * pow(t, i) * pow((1 - t), (n - i));
	}
	return x;
}

float bezier_curves::Bezier::get_y_arbitrary(float t)
{
	float y = 0.0f;
	for (int i = 0; i < n; i++)
	{
		y = y + p[i]->getPoint()->Y * (factorial(n) / (factorial(i) * factorial(n - i))) * pow(t, i) * pow((1 - t), (n - i));
	}
	return y;
}

System::Void bezier_curves::Bezier::draw_arbitrary_order(Graphics^ im)
{
	float t = 0.005f;
	//array<PointF>^ arr = gcnew array<PointF>(1 / t + 1);
	//int i = 0;
	for (float cur_t = 0; cur_t < 1; cur_t += t)
	{
		float x = get_x_arbitrary(cur_t);
		float y = get_y_arbitrary(cur_t);
		im->FillRectangle(gcnew SolidBrush(Color::Black), x, y, 1.0f, 1.0f);
		//arr[i++] = PointF(x, y);
	}
	//im->DrawCurve(gcnew Pen(Color::Black), arr);
}

System::Void bezier_curves::Bezier::draw_third_order(Graphics^ im)
{
	for (int i = 4; i <= p->Count; i += 3)
	{
		b = PointF(p[i - 1]->getPoint()->X, p[i - 1]->getPoint()->Y);
		for (float cur_t = 0; cur_t < 1; cur_t += 0.005f)
		{
			float x = get_x_third(cur_t, i);
			float y = get_y_third(cur_t, i);
			im->DrawRectangle(gcnew Pen(Color::Black), x, y, 1.0f, 1.0f);
		}
	}
}
