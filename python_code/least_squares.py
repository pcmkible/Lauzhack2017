import numpy as np
pad = lambda x: np.hstack([x, np.ones((x.shape[0], 1))])
unpad = lambda x: x[:,:-1]
def calc_A(v1,v2,v3,v1_img,v2_img,v3_img):
    primary = np.array([v1,v2,v3])
    secondary = np.array([v1_img,v2_img,v3_img])

    n = primary.shape[0]
    # Pad the data with ones, so that our transformation can do translations too
    X = pad(primary)
    Y = pad(secondary)
    # Solve the least squares problem X * A = Y
    # to find our transformation matrix A
    A, res, rank, s = np.linalg.lstsq(X, Y)
    return A
A=calc_A(v1,v2,v3,v1_img,v2_img,v3_img)
transform = lambda x: unpad(np.dot(pad(x), A))

print transform(primary)
