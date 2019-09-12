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