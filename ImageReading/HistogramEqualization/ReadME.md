## GrayHistogram.cpp
�Ҷ�ͼ�����������ļ�

DisplayHistogram1:
1.Ԥ����ֱ��ͼ������������Ҫ�ı���ֵ��
2.�����ݴ���ֱ��ͼ��calcHist����������ʹ�÷�ʽ������Ŀʹ�õ�һ�ֺ������巽ʽ��
calcHist( const Mat* images, //�������ݣ�����ͼ������ͬʱ�������ͼƬ
    int nimages, //����ͼƬ����
    const int* channels, //��������������ֱ��ͼʱ��ʹ�����������е���Щͨ��
    InputArray mask, //ѡ����������Щ���ز���ֱ��ͼ���㡣��ʹ�ÿɸ�ֵΪMat()����noArray
    OutputArray hist, //ֱ��ͼ����Ľ��
    int dims, //���ֱ��ͼ��ά�ȣ���channels��Ԫ�ظ�����ͬ
    const int* histSize, //ָ����ά����Ҫ��Ϊ���ٸ����䣬�Ҷ�ͼ�����ط�Χ0-255������ʹ��255����ÿ������������ͳ��
    const float** ranges,  //���ȵ���dims���������䳤��
    bool uniform = true,  
    bool accumulate = false 
);
3.����Ҷ�ֱ��ͼ�������飬Mat���͡���Ϊscale*size����Ϊsize����ɫͨ��Ϊ1��
4.���ֱ��ͼ������������Сֵ�����ֵ��
5.����ֱ��ͼ������Ի�ȡÿ������ֵ�Ķ�Ӧ��������ʹ��rectangle��������ֱ��ͼ��
6.��ʾ�Ҷ�ͼ

## Color.cpp
��ɫͼ����

ConvertColor2Gray��
����cvtColor����ʵ�ֲ�ɫͼ��ת�Ҷ�ͼ��
ConvertColor2Gray1:
�Բ�ɫͼ��RGB��ͨ�����ֵ������ɫͼ��ת��Ϊ�Ҷ�ͼ��
GetChannel:
ʹ��split��������ɫͼ���з�ΪBGR������ͨ�����顣
ʹ��merge������������ͨ������ϲ�Ϊһ�Ų�ɫͼ��

