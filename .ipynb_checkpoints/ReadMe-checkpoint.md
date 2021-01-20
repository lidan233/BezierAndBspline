# calculateBezierandBspline 

## Contents 
In this project, I implement a framework about Bezier and Bspline. 
在这个项目中，我对课程中有关Bezier和Bspline的项目集中实现。我选择使用Bspline曲线去拟合一个球，并根据errorDistance去判定拟合标准。 

## Envirments 
AMD 3800 cpu, rtx3090. 

## Results 
- 从球上，按参数方程（对应地球经纬度模型），从原始球模型采样10*10个点，然后基于这些点进行BSpline曲线拟合，考虑到可以使用不同的degree去拟合，
- 因此我将degree分别设为3-14去计算拟合精度。 拟合精度可以通过Bspline插值的点与这些点到球面的最小距离的和计算。 
- 计算统计结果如下所示：
- 
## result 
