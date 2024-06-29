@author:FLY
@software:PyCharm
@time:2017/08/24
"""
import pickle
from os import path
import jieba
import matplotlib.pyplot as plt
from wordcloud import WordCloud, STOPWORDS, ImageColorGenerator
text = ''
with open('�˹����ܼ�Ӧ��.txt', 'r', encoding='utf8') as fin:
    for line in fin.readlines():
        line = line.strip('\n')
# sep��.join��seq����sep��Ϊ�ָ�������seq���е�Ԫ�غϲ���һ���µ��ַ���
text += ' '.join(jieba.cut(line))
backgroud_Image = plt.imread('man.jpg')
print('����ͼƬ�ɹ���')
'''���ô�����ʽ'''
wc = WordCloud(
    background_color='white',# ���ñ�����ɫ
    mask=backgroud_Image,# ���ñ���ͼƬ
    font_path='C:\Windows\Fonts\STZHONGS.TTF',  # ���������ĵĻ��������������ӣ���Ȼ����ַ��򣬲����ֺ���
    max_words=2000, # ���������ʵ������
    stopwords=STOPWORDS,# ����ͣ�ô�
    max_font_size=150,# �����������ֵ
    random_state=30# �����ж������������״̬�����ж�������ɫ����
)
wc.generate_from_text(text)
print('��ʼ�����ı�')
#�ı�������ɫ
img_colors = ImageColorGenerator(backgroud_Image)
#������ɫΪ����ͼƬ����ɫ
wc.recolor(color_func=img_colors)
# ��ʾ����ͼ
plt.imshow(wc)
# �Ƿ���ʾx�ᡢy���±�
plt.axis('off')
plt.show()
# ���ģ�����ڵ�·����
d = path.dirname(__file__)
# os.path.join()��  �����·����Ϻ󷵻�
wc.to_file(path.join(d, "h11.jpg"))
print('���ɴ��Ƴɹ�!')