import re
from collections import OrderedDict

with open('Ankle data running back and forth.txt') as f:
    lines = f.readlines() 

lines = (" ").join(lines)

ankle_right = re.findall(r'AR.+(?=A)|AR\S*', lines)

ankle_right = re.findall(r'(?<=,)[-\w*]*(?=\S*)',str(ankle_right))    

for i in ankle_right:
    if len(i) == 0:
        ankle_right.remove(i)
    else:
        continue

ankle_right_x = ankle_right[::3]
ankle_right_y = ankle_right[1::3]
ankle_right_z = ankle_right[2::3]
ankle_right_z = [float(i) for i in ankle_right_z]
k = 0
diff = []
for i in range(0,len(ankle_right_z)):
    if ankle_right_z[i] < 0:
       j = (180 - abs(ankle_right_z[i]))+180
       ankle_right_z[i] = j
        

#%%
ankle_left1 = re.findall(r'AL.+(?=A)|AL\S*', lines)

ankle_left = re.findall(r'(?<=,)[-\w*]*(?=\S*)',str(ankle_left1))    

for i in ankle_left:
    if len(i) == 0:
        ankle_left.remove(i)
    else:
        continue

ankle_left_x = ankle_left[::3]
ankle_left_y = ankle_left[1::3]
ankle_left_z = ankle_left[2::3]
ankle_left_z = [float(i) for i in ankle_left_z]

#%%
import matplotlib.pyplot as plt
import numpy as np

x = np.arange(0,1,0.01)

plt.figure()
plt.plot(x, ankle_right_z[0:100])
plt.plot(x, ankle_left_z[0:100])
plt.title("Relative Ankle Swing Rotation (θ) Vs Time (s)")
plt.ylabel("Relative Ankle Swing Rotation (θ)")
plt.xlabel("Time (s)")

#%%
with open('Knee 30 sec test.txt') as f:
    lines = f.readlines() 

lines = (" ").join(lines)

knee_right = re.findall(r'KR.+(?=K)|KR\S*', lines)

knee_right = re.findall(r'(?<=,)[-\w*]*(?=\S*)',str(knee_right))    

for i in knee_right:
    if len(i) == 0:
        knee_right.remove(i)
    else:
        continue

knee_right_x = knee_right[::3]
knee_right_x = [float(i) for i in knee_right_x]
knee_right_y = knee_right[1::3]
knee_right_z = knee_right[2::3]
knee_right_z = [float(i) for i in knee_right_z]

# for i in range(0,len(knee_right_z)):
#     if knee_right_z[i] < 0:
#        j = (180 - abs(knee_right_z[i]))+180
#        knee_right_z[i] = j
        

#%%
knee_left1 = re.findall(r'KL.+(?=K)|KL\S*', lines)

knee_left = re.findall(r'(?<=,)[-\w*]*(?=\S*)',str(knee_left1))    

for i in knee_left:
    if len(i) == 0:
        knee_left.remove(i)
    else:
        continue

knee_left_x = knee_left[::3]
knee_left_x = [float(i) for i in knee_left_x]
knee_left_y = knee_left[1::3]
knee_left_z = knee_left[2::3]
knee_left_z = [float(i) for i in knee_left_z]
#%%
x = np.arange(0,20,0.01)

def movingaverage(interval, window_size):
    window= np.ones(int(window_size))/float(window_size)
    return np.convolve(interval, window, 'same')


plt.figure()
y = movingaverage(knee_right_x[0:2000],5)
plt.plot(x, y)
plt.plot(x, knee_left_x[0:2000])
plt.title("Relative Knee Flexion (θ) Vs Time (s)")
plt.ylabel("Relative Knee Flexion (θ)")
plt.xlabel("Time (s)")

#%%
with open('Hip back and forth2.txt') as f:
    lines = f.readlines() 

lines = (" ").join(lines)

hip_right2 = re.findall(r'HR.+(?=H)|HR\S*', lines)

hip_right = re.findall(r'(?<=,)[-\w*]*(?=\S*)',str(hip_right2))    

for i in hip_right:
    if len(i) == 0:
        hip_right.remove(i)
    else:
        continue

hip_right_x = hip_right[::3]
hip_right_x = [float(i) for i in hip_right_x]
hip_right_y = hip_right[1::3]
hip_right_z = hip_right[2::3]
hip_right_z = [float(i) for i in hip_right_z]

# for i in range(0,len(knee_right_z)):
#     if knee_right_z[i] < 0:
#        j = (180 - abs(knee_right_z[i]))+180
#        knee_right_z[i] = j
        

#%%
hip_left1 = re.findall(r'HL.+(?=H)|HL\S*', lines)

hip_left = re.findall(r'(?<=,)[-\w*]*(?=\S*)',str(hip_left1))    

for i in hip_left:
    if len(i) == 0:
        hip_left.remove(i)
    else:
        continue

hip_left_x = hip_left[::3]
hip_left_x = [float(i) for i in hip_left_x]
hip_left_y = hip_left[1::3]
hip_left_z = hip_left[2::3]
hip_left_z = [float(i) for i in hip_left_z]
for i in range(0,len(hip_left_x)):
    j = hip_right_x[i] + 100
    hip_right_x[i] = j

#%%
x = np.arange(0,1,0.01)

def movingaverage(interval, window_size):
    window= np.ones(int(window_size))/float(window_size)
    return np.convolve(interval, window, 'same')

plt.figure()
plt.plot(x, hip_right_x[500:600])
plt.plot(x, hip_left_x[500:600])
plt.title("Relative Hip Alignment (θ) Vs Time (s)")
plt.ylabel("Relative Hip Alignment (θ)")
plt.xlabel("Time (s)")