import numpy as np
from math import sin, cos, radians


def rotate_x(angle, point):
    angle = radians(angle)
    points_as_np = np.array((*point, 1))
    rotate_x_mat = np.array([
        [1, 0, 0, 0],
        [0, cos(angle), sin(angle), 0],
        [0, -sin(angle), cos(angle), 0],
        [0, 0, 0, 1]
    ])
    return np.matmul(rotate_x_mat, points_as_np)


def rotate_y(angle, point):
    angle = radians(angle)
    points_as_np = np.array((*point, 1))
    rotate_y_mat = np.array([
        [cos(angle), 0, -sin(angle), 0],
        [0, 1, 0, 0],
        [sin(angle), 0, cos(angle), 0],
        [0, 0, 0, 1]
    ])
    return np.matmul(rotate_y_mat, points_as_np)


def rotate_z(angle, point):
    angle = radians(angle)
    points_as_np = np.array((*point, 1))
    rotate_z_mat = np.array([
        [cos(angle), -sin(angle), 0, 0],
        [sin(angle), cos(angle), 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ])
    return np.matmul(rotate_z_mat, points_as_np)


def translate(x_trans, y_trans, z_trans, point):
    points_as_np = np.array((*point, 1))
    translation_mat = np.array([
        [1, 0, 0, x_trans],
        [0, 1, 0, y_trans],
        [0, 0, 1, z_trans],
        [0, 0, 0, 1]
    ])
    return np.matmul(translation_mat, points_as_np)

def scale(x_scale, y_scale, z_scale, point):
    points_as_np = np.array((*point, 1))
    scale_mat = np.array([
        [x_scale, 0, 0, 0],
        [0, y_scale, 0, 0],
        [0, 0, z_scale, 0],
        [0, 0, 0, 1]
    ])
    return np.matmul(scale_mat, points_as_np)



file = open("input_obj_file")
file_w = open("output.obj", "w")

v = []

triangles = 0

for line in file:
    if line.startswith("v "):
        ir = line.strip().replace("f ", "").replace("\n", "").split(" ")
        x = float(ir[1])
        y = float(ir[2])
        z = float(ir[3])

        result = scale(1, 1, 1, (x, y, z))

        # result = translate(30, -50, -60, result[:3])
        x, y, z, _ = list(map(lambda y: format(y, 'f'), result))
        file_w.write(f"v {x} {y} {z}\n")

    if line.startswith("f"):
        triangles+=1
        splitter = '/'
        _, vert1, vert2, vert3 = line.split()
        line_ = f"f {vert1.split(splitter)[0]} {vert2.split(splitter)[0]} {vert3.split(splitter)[0]}\n"
        file_w.write(line_)

print(f"Number of triangles is {triangles}")

file_w.close()
