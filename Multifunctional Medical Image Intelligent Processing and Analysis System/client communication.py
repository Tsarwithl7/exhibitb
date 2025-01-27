import requests
import nibabel
from io import BytesIO
import datetime
import json

REGION_GROW = 1
REGION_SPLIT = 2
WATERSHED = 3
SAM = 4
CEPH_LOCATE = 5
CBCT_TEETH = 6
CBCT_BONE = 7
FACIAL_MATCH = 8
TEETH_SEG2 = 9
SAM_INTERACTIVE = 10
SAM_HQ = 11
"""
:arg
REGION_GROW     区域生长算法
REGION_SPLIT    区域分裂合并
WATERSHED       分水岭
SAM             SAM
CEPH_LOCATE     头影侧位片
CBCT_TEETH      CBCT牙齿
CBCT_BONE       CBCT骨骼
FACIAL_MATCH    骨骼与面部匹配
TEETH_SEG2      提取特定牙齿nii文件
SAM_INTERACTIVE 交互SAM
SAM_HQ          使用sam_hq模型进行分割
"""


def alg(algorithm, param, file_path, file_path2):
    if algorithm == 1:  # 区域生长
        url = 'http://211.87.232.167:5000/region_grow'
        files = {
            'file': ('region_grow_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'region_grow'}

    elif algorithm == 2:  # 区域分裂合并
        url = 'http://211.87.232.167:5000/region_split'
        files = {
            'file': ('region_split_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'region_split'}

    elif algorithm == 3:  # 分水岭算法
        url = 'http://211.87.232.167:5000/watershed'
        files = {
            'file': ('watershed_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'watershed'}

    elif algorithm == 4:  # segment_anything
        url = 'http://211.87.232.167:5000/sam'
        files = {
            'file': ('sam_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'sam'}

    elif algorithm == 5:  # 投影侧位片关键点检测
        url = 'http://211.87.232.167:5000/ceph_locate'
        files = {
            'file': ('ceph_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'ceph'}

    elif algorithm == 6:  # cbct牙齿分割
        url = 'http://211.87.232.167:5000/cbct_teeth'
        files = {
            'file': ('teeth1_temp.nii',
                     open(file_path, 'rb'),
                     'cbct/nii',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'cbct_teeth'}

    elif algorithm == 7:  # cbct骨骼分割
        url = 'http://211.87.232.167:5000/cbct_bone'
        files = {
            'file': ('bone_temp.nii',
                     open(file_path, 'rb'),
                     'cbct/nii',
                     {'Expires': '0'}
                     )
        }
        data = {'name': 'cbct_bone'}

    elif algorithm == 8:  # 融合
        url = 'http://211.87.232.167:5000/facial_match'
        files = {
            'skull_file': ('skull_temp.JPG',
                           open(file_path, 'rb'),
                           'image/JPG',
                           {'Expires': '0'}
                           ),
            'face_file': ('face_temp.JPG',
                          open(file_path2, 'rb'),
                          'image/JPG',
                          {'Expires': '0'}
                          )
        }
        data = {'name': 'facial_match'}

    elif algorithm == 9:  # 特定牙齿nii文件
        url = 'http://211.87.232.167:5000/teeth_seg2'
        files = {
            'file': ('teeth2_temp.nii',
                     open(file_path, 'rb'),
                     'cbct/nii',
                     {'Expires': '0'}
                     ),

        }
        data = {'param': param}

    elif algorithm == 10:  # 交互式sam
        url = 'http://211.87.232.167:5000/sam_interactive'
        files = {
            'file': ('sam_interactive_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     ),

        }
        data = {
            'param': json.dumps(param)
        }
    elif algorithm == 11:  # sam_hq
        url = 'http://211.87.232.167:5000/sam_hq'
        files = {
            'file': ('sam_hq_temp.JPG',
                     open(file_path, 'rb'),
                     'image/JPG',
                     {'Expires': '0'}
                     ),

        }
        if param is not None:
            data = {
                'param': json.dumps(param),
                'type': 'param_seg'
            }
        else:
            data = {
                'type': 'global_seg'
            }
    # data = {'points': param['points'], 'box': param['box']}

    r = requests.post(url, data=data, files=files)

    print(r.headers)
    current_time = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
    if algorithm == 1 or algorithm == 2 or algorithm == 3 or algorithm == 4 or algorithm == 8 or algorithm == 10 or algorithm == 11:
        buf = BytesIO(r.content)
        with open('C:/Users/sun13/Desktop/TEST/return.JPG', 'wb') as out:
            out.write(buf.read())
    elif algorithm == 5:
        print(r.text)
        return r.text
    else:
        print(r.content)
        with open('C:/Users/sun13/Desktop/TEST/return.nii.gz', 'wb') as out:
            out.write(r.content)
        return r
