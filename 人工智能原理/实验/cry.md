## Conv

### im2col

加速卷积运算

原代码中实现的卷积采用的是暴力循环计算卷积结果的方式，运算速度较慢

```python
class Conv():
    """
    Conv layer
    """
    def __init__(self, Cin, Cout, F, stride=1, padding=0, bias=True):
        self.Cin = Cin
        self.Cout = Cout
        self.F = F
        self.S = stride
        #self.W = {'val': np.random.randn(Cout, Cin, F, F), 'grad': 0}
        self.W = {'val': np.random.normal(0.0,np.sqrt(2/Cin),(Cout,Cin,F,F)), 'grad': 0} # Xavier Initialization
        self.b = {'val': np.random.randn(Cout), 'grad': 0}
        self.cache = None
        self.pad = padding

    def _forward(self, X):
        X = np.pad(X, ((0,0),(0,0),(self.pad,self.pad),(self.pad,self.pad)), 'constant')
        (N, Cin, H, W) = X.shape
        H_ = H - self.F + 1
        W_ = W - self.F + 1
        Y = np.zeros((N, self.Cout, H_, W_))

        for n in range(N):
            for c in range(self.Cout):
                for h in range(H_):
                    for w in range(W_):
                        Y[n, c, h, w] = np.sum(X[n, :, h:h+self.F, w:w+self.F] * self.W['val'][c, :, :, :]) + self.b['val'][c]

        self.cache = X
        return Y

    def _backward(self, dout):
        # dout (N,Cout,H_,W_)
        # W (Cout, Cin, F, F)
        X = self.cache
        (N, Cin, H, W) = X.shape
        H_ = H - self.F + 1
        W_ = W - self.F + 1
        W_rot = np.rot90(np.rot90(self.W['val']))

        dX = np.zeros(X.shape)
        dW = np.zeros(self.W['val'].shape)
        db = np.zeros(self.b['val'].shape)

        # dW
        for co in range(self.Cout):
            for ci in range(Cin):
                for h in range(self.F):
                    for w in range(self.F):
                        dW[co, ci, h, w] = np.sum(X[:,ci,h:h+H_,w:w+W_] * dout[:,co,:,:])

        # db
        for co in range(self.Cout):
            db[co] = np.sum(dout[:,co,:,:])

        dout_pad = np.pad(dout, ((0,0),(0,0),(self.F,self.F),(self.F,self.F)), 'constant')
        #print("dout_pad.shape: " + str(dout_pad.shape))
        # dX
        for n in range(N):
            for ci in range(Cin):
                for h in range(H):
                    for w in range(W):
                        #print("self.F.shape: %s", self.F)
                        #print("%s, W_rot[:,ci,:,:].shape: %s, dout_pad[n,:,h:h+self.F,w:w+self.F].shape: %s" % ((n,ci,h,w),W_rot[:,ci,:,:].shape, dout_pad[n,:,h:h+self.F,w:w+self.F].shape))
                        dX[n, ci, h, w] = np.sum(W_rot[:,ci,:,:] * dout_pad[n, :, h:h+self.F,w:w+self.F])

        return dX
```

可以通过im2col的方式加速卷积的运算

```python
class Conv:
    def __init__(self, in_channels, out_channels, kernel_size, stride=1):
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = (kernel_size,kernel_size,in_channels,out_channels)
        self.stride = stride
        self.cache = 0
        
        self.W = {'val': np.random.standard_normal((self.kernel_size)), 'grad': np.zeros(self.kernel_size)}
        self.b = {'val': np.random.standard_normal(out_channels), 'grad': np.zeros(out_channels)}
        
    def _forward(self, x):
        (N,Cin,H,W) = x.shape
        self.input_shape = x.shape
        H_ = int((H - self.kernel_size[0]) / self.stride + 1)
        W_ = int((W - self.kernel_size[0]) / self.stride + 1)
        col_weights = self.W['val'].reshape([-1,self.out_channels])
        self.col_image = []
        conv_out = np.zeros((N,self.out_channels,H_,W_))
        for i in range(N):
            img_i = x[i]
            self.col_image_i = self.im2col(img_i,self.kernel_size[0],self.stride)
#             print(self.col_image_i.shape)
            conv_out[i] = np.reshape(np.dot(self.col_image_i, col_weights) + self.b['val'],(H_,W_,-1)).transpose(2,0,1)
            self.col_image.append(self.col_image_i)
        self.col_image = np.array(self.col_image)
        return conv_out
    
    def _backward(self, dout):
        (N,Cout,H_,W_) = dout.shape
        col_dout = dout.reshape((N,Cout,-1))
        
        for i in range(self.input_shape[0]):
            self.W['grad'] += np.dot(col_dout[i], self.col_image[i]).reshape(self.W['val'].shape)
        self.b['grad'] += np.sum(col_dout,axis=(0,2)).reshape(self.b['val'].shape)
        
        dout_pad = np.pad(dout, ((0, 0), (0, 0), (self.kernel_size[0] - 1, self.kernel_size[0] - 1), (self.kernel_size[0] - 1, self.kernel_size[0] - 1)), 'constant', constant_values=0)
        
        weight_flip = self.W['val'][:,:,::-1,::-1]
        weight_flip = np.swapaxes(weight_flip, 0, 1)
        weight_flip_col = weight_flip.reshape(self.in_channels, -1)
        
        next_dout =  np.zeros((N, self.in_channels, self.input_shape[2], self.input_shape[3]))
        for i in range(N):
            dout_pad_image_batch_i = dout_pad[i,:]
            dout_pad_image_batch_i_col = self.im2col(dout_pad_image_batch_i , self.kernel_size[0], self.stride)
            next_dout[i] = np.reshape(np.dot(weight_flip_col, np.transpose(dout_pad_image_batch_i_col)), (self.in_channels, self.input_shape[2], self.input_shape[3]))
        
        return next_dout
        
    def im2col(self, image, ksize, stride):
        # image is a 3d tensor([channel, height, width])
        image_col = []
        for i in range(0, image.shape[1] - ksize + 1, stride):
            for j in range(0, image.shape[2] - ksize + 1, stride):
                col = image[:,i:i + ksize, j:j + ksize].reshape([-1])
                image_col.append(col)
        image_col = np.array(image_col)
        return image_col
```



## Softmax

减去最大值防止上溢出

## Gradient Checking

导数定义的衍生：
$$
\lim_{\epsilon \to 2}\frac{f(x+\epsilon)-f(x-\epsilon)}{2\epsilon}
$$
通过这个公式可以近似求得导数从而进行`Gradient Check`，通过更改$\epsilon$的大小，就可以控制`Gradient Check`的精度

对于神经网络中的参数$\theta$，其近似导数为
$$
d\theta_{apprpx}=\frac{J(\theta+\epsilon)-J(\theta-\epsilon)}{2\epsilon}
$$
然后代入下面的公式进行`Gradient Check`
$$
\frac{||d\theta_{approx}-d\theta||}{||d\theta_{approx}||_2+||d\theta||_2}
$$

### Code

```python
import numpy as np

def forward_propagation(x, theta):
    J = np.dot(theta, x)
    return J

x, theta = 2, 4
J = forward_propagation(x, theta)
print ("J = " + str(J))

def backward_propagation(x, theta):
    dtheta = x
    return dtheta

x, theta = 2, 4
dtheta = backward_propagation(x, theta)
print ("dtheta = " + str(dtheta))

def gradient_check(x, theta, epsilon=1e-7):
    thetaplus = theta + epsilon
    thetaminus = theta - epsilon
    
    J_plus = forward_propagation(x, thetaplus)
    J_minus = forward_propagation(x, thetaminus)
    
    gradapprox = (J_plus - J_minus) / (2 * epsilon)
    
    # Check if gradapprox is close enough to backward propagation
    grad = backward_propagation(x, theta)
    
    numerator = np.linalg.norm(grad - gradapprox)
    denominator = np.linalg.norm(grad) + np.linalg.norm(gradapprox)
    difference = numerator / denominator
    
    if difference < 1e-7:
        print('The gradient is correct')
    else:
        print('The gradient is wrong')
        
    return difference

x, theta = 2, 4
difference = gradient_check(x, theta)
print("difference = " + str(difference))
```

## 参考

[How to Debug a Neural Network With Gradient Checking](https://towardsdatascience.com/how-to-debug-a-neural-network-with-gradient-checking-41deec0357a9)