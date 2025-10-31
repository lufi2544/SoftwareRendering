# 3D Graphics Concepts - Daily Review Checklist

## Coordinate Spaces & Transformations

### 1. The Four Main Coordinate Spaces

#### Model/Local Space
- [ ] **Definition**: Vertices defined relative to mesh origin (0,0,0)
- [ ] **Context**: This is where the artist creates the model. A cube might have vertices at (-1,-1,-1) to (1,1,1) centered at its own origin
- [ ] **Example**: Your cube vertices before any transformations

#### World Space
- [ ] **Definition**: After applying model matrix (scale, rotation, translation)
- [ ] **Context**: All objects placed in the same global coordinate system. This is where objects exist in your scene
- [ ] **Formula**: `V_world = ModelMatrix × V_local`
- [ ] **Example**: If cube is at position (10, 5, 20) in the world, vertices are transformed relative to that position

#### View/Camera Space
- [ ] **Definition**: After applying view matrix (camera at origin)
- [ ] **Context**: The world transformed so the camera is at (0,0,0) looking down +Z (or -Z depending on system)
- [ ] **Formula**: `V_view = ViewMatrix × V_world`
- [ ] **Key insight**: Camera is ALWAYS at (0, 0, 0) in this space!

#### Clip/Screen Space
- [ ] **Definition**: After applying projection matrix
- [ ] **Context**: 3D coords mapped to 2D screen with depth info preserved
- [ ] **Formula**: `V_clip = ProjectionMatrix × V_view`
- [ ] **Next step**: Perspective divide (x/w, y/w, z/w) to get NDC (Normalized Device Coordinates)


### 2. Key Rule: NEVER Mix Coordinate Spaces

- [ ] **Why?** Each space has different origins and orientations. Mixing them is like adding meters and feet
- [ ] **Example of bug**: `distance = world_vertex - view_space_camera` ← WRONG! Different coordinate systems
- [ ] **Correct**: Ensure all vectors are in same space before operations
  ```
  WRONG: dot(view_normal, world_camera_pos - view_vertex)
  RIGHT: dot(view_normal, (0,0,0) - view_vertex)
  ```
- [ ] When doing calculations (dot products, distances, directions), ALL vectors must be in the SAME space
- [ ] In view space, camera is ALWAYS at (0, 0, 0)
- [ ] Each transformation matrix converts from one space to the next


### 3. Transformation Pipeline Order

```
Vertex → [Model Matrix] → World Space → [View Matrix] → View Space → [Projection Matrix] → Clip Space
```

#### Model Matrix (Local → World)
- [ ] **Order**: `M = Translation × Rotation × Scale` (applied right to left)
- [ ] **Why this order?**
  - Scale first: Makes object bigger/smaller around its own origin
  - Rotate second: Rotates around origin (after scaling)
  - Translate last: Moves to world position
- [ ] **Wrong order**: Translating first would rotate around wrong point!

#### View Matrix (World → View)
- [ ] **Purpose**: Moves entire world so camera is at origin
- [ ] **Result**: Everything positioned relative to camera

#### Projection Matrix (View → Clip)
- [ ] **Purpose**: Applies perspective or orthographic projection
- [ ] **Result**: 3D to 2D transformation with depth preserved


## Matrices & Transformations

### 4. Matrix Multiplication Order Matters

- [ ] **Key rule**: Matrices multiply RIGHT to LEFT
- [ ] **Example**: `C = B × A` means "apply A first, then B"
- [ ] **For transformations**: `World = T × R × S` means:
  1. First Scale (S)
  2. Then Rotate (R)
  3. Then Translate (T)
- [ ] **For vectors**: `v' = Matrix × v` (matrix on left, vector on right)
- [ ] **Not commutative**: `A × B ≠ B × A` (order matters!)


### 5. Common Matrix Types with Formulas

**Note**: All matrices are 4x4 to support homogeneous coordinates (x, y, z, w).
Matrix-vector multiplication: `v' = M × v` where v = [x, y, z, 1]ᵀ

#### Translation Matrix
- [ ] **Purpose**: Moves/translates objects in space by offset (tx, ty, tz)
- [ ] **Matrix (4×4)**:
```
     Column: 0   1   2   3
Row 0:     [ 1   0   0   tx ]
Row 1:     [ 0   1   0   ty ]
Row 2:     [ 0   0   1   tz ]
Row 3:     [ 0   0   0   1  ]

In code (column-major):
T.m[0][0]=1;  T.m[1][0]=0;  T.m[2][0]=0;  T.m[3][0]=tx;
T.m[0][1]=0;  T.m[1][1]=1;  T.m[2][1]=0;  T.m[3][1]=ty;
T.m[0][2]=0;  T.m[1][2]=0;  T.m[2][2]=1;  T.m[3][2]=tz;
T.m[0][3]=0;  T.m[1][3]=0;  T.m[2][3]=0;  T.m[3][3]=1;
```
- [ ] **How it works**: Adds tx to x, ty to y, tz to z
- [ ] **Example**: Translate by (5, 10, 15)
```
Input vertex:  v = (2, 3, 4, 1)
Output vertex: v' = (2+5, 3+10, 4+15, 1) = (7, 13, 19, 1)
```

#### Scale Matrix
- [ ] **Purpose**: Makes objects bigger (sx>1) or smaller (sx<1) along each axis
- [ ] **Matrix (4×4)**:
```
     [ sx  0   0   0 ]
     [ 0   sy  0   0 ]
     [ 0   0   sz  0 ]
     [ 0   0   0   1 ]

In code:
S.m[0][0]=sx; S.m[1][1]=sy; S.m[2][2]=sz; S.m[3][3]=1;
All other elements = 0
```
- [ ] **How it works**: Multiplies x by sx, y by sy, z by sz
- [ ] **Example**: Double size → sx=2, sy=2, sz=2
```
Input vertex:  v = (1, 2, 3, 1)
Output vertex: v' = (1×2, 2×2, 3×2, 1) = (2, 4, 6, 1)
```

#### Rotation Matrix - Around X axis
- [ ] **Purpose**: Rotates around X axis by angle θ (in radians)
- [ ] **Matrix (4×4)**:
```
     [ 1     0        0       0 ]
Rx = [ 0   cos(θ)  -sin(θ)   0 ]
     [ 0   sin(θ)   cos(θ)   0 ]
     [ 0     0        0       1 ]

In code:
float c = cos(theta);
float s = sin(theta);
Rx.m[0][0]=1; Rx.m[1][0]=0; Rx.m[2][0]=0; Rx.m[3][0]=0;
Rx.m[0][1]=0; Rx.m[1][1]=c; Rx.m[2][1]=s; Rx.m[3][1]=0;
Rx.m[0][2]=0; Rx.m[1][2]=-s; Rx.m[2][2]=c; Rx.m[3][2]=0;
Rx.m[0][3]=0; Rx.m[1][3]=0; Rx.m[2][3]=0; Rx.m[3][3]=1;
```
- [ ] **Right-hand rule**: Thumb along +X, fingers curl in rotation direction
- [ ] **Example**: Rotate 90° (π/2 radians) around X
```
cos(90°) = 0, sin(90°) = 1
Point (0, 1, 0) → (0, 0, 1)  [Y axis rotates to Z axis]
```

#### Rotation Matrix - Around Y axis
- [ ] **Purpose**: Rotates around Y axis by angle θ (in radians)
- [ ] **Matrix (4×4)**:
```
     [  cos(θ)   0   sin(θ)   0 ]
Ry = [    0      1     0      0 ]
     [ -sin(θ)   0   cos(θ)   0 ]
     [    0      0     0      1 ]

In code:
float c = cos(theta);
float s = sin(theta);
Ry.m[0][0]=c;  Ry.m[1][0]=0; Ry.m[2][0]=-s; Ry.m[3][0]=0;
Ry.m[0][1]=0;  Ry.m[1][1]=1; Ry.m[2][1]=0;  Ry.m[3][1]=0;
Ry.m[0][2]=s;  Ry.m[1][2]=0; Ry.m[2][2]=c;  Ry.m[3][2]=0;
Ry.m[0][3]=0;  Ry.m[1][3]=0; Ry.m[2][3]=0;  Ry.m[3][3]=1;
```
- [ ] **Note**: Sign of sin terms differs from Rx and Rz (because of Y axis orientation)
- [ ] **Example**: Rotate 90° around Y
```
Point (1, 0, 0) → (0, 0, -1)  [X axis rotates to -Z axis]
```

#### Rotation Matrix - Around Z axis
- [ ] **Purpose**: Rotates around Z axis by angle θ (in radians)
- [ ] **Matrix (4×4)**:
```
     [ cos(θ)  -sin(θ)   0   0 ]
Rz = [ sin(θ)   cos(θ)   0   0 ]
     [   0        0      1   0 ]
     [   0        0      0   1 ]

In code:
float c = cos(theta);
float s = sin(theta);
Rz.m[0][0]=c; Rz.m[1][0]=-s; Rz.m[2][0]=0; Rz.m[3][0]=0;
Rz.m[0][1]=s; Rz.m[1][1]=c;  Rz.m[2][1]=0; Rz.m[3][1]=0;
Rz.m[0][2]=0; Rz.m[1][2]=0;  Rz.m[2][2]=1; Rz.m[3][2]=0;
Rz.m[0][3]=0; Rz.m[1][3]=0;  Rz.m[2][3]=0; Rz.m[3][3]=1;
```
- [ ] **Example**: Rotate 90° around Z
```
Point (1, 0, 0) → (0, 1, 0)  [X axis rotates to Y axis]
```

#### View Matrix (Look-At)
- [ ] **Purpose**: Transforms world to camera/view space. Camera at 'eye', looking at 'target', with 'up' direction
- [ ] **Step 1 - Calculate basis vectors**:
```
// Forward (direction camera is looking)
forward = normalize(target - eye)

// Right (perpendicular to forward and up)
right = normalize(cross(forward, up))

// Up (perpendicular to right and forward)
up_actual = cross(right, forward)
```
- [ ] **Step 2 - Build view matrix (4×4)**:
```
View = [ right.x     right.y     right.z     -dot(right, eye)    ]
       [ up.x        up.y        up.z        -dot(up, eye)       ]
       [ -forward.x  -forward.y  -forward.z   dot(forward, eye)  ]
       [ 0           0           0            1                  ]

In code:
// Build rotation part (inverse of camera orientation)
V.m[0][0]=right.x;    V.m[1][0]=right.y;    V.m[2][0]=right.z;
V.m[0][1]=up.x;       V.m[1][1]=up.y;       V.m[2][1]=up.z;
V.m[0][2]=-forward.x; V.m[1][2]=-forward.y; V.m[2][2]=-forward.z;

// Build translation part (move world by -eye)
V.m[3][0]=-dot(right, eye);
V.m[3][1]=-dot(up, eye);
V.m[3][2]=dot(forward, eye);

// Homogeneous row
V.m[0][3]=0; V.m[1][3]=0; V.m[2][3]=0; V.m[3][3]=1;
```
- [ ] **Why negative?**: We're moving the world opposite to where camera moved
- [ ] **Result**: Camera ends up at (0,0,0) looking down +Z (or -Z depending on convention)

#### Perspective Projection Matrix
- [ ] **Purpose**: Applies perspective foreshortening (things farther = smaller)
- [ ] **Parameters**:
  - `fov`: Field of view angle (in radians, typically π/4 to π/3)
  - `aspect`: Screen aspect ratio (width/height, e.g., 16/9 = 1.778)
  - `near`: Near clipping plane distance (e.g., 0.1)
  - `far`: Far clipping plane distance (e.g., 100.0)

- [ ] **Step 1 - Calculate focal length**:
```
f = 1.0 / tan(fov / 2.0)

Example: fov = 90° = π/2 radians
f = 1.0 / tan(π/4) = 1.0 / 1.0 = 1.0
```

- [ ] **Step 2 - Build projection matrix (4×4)**:
```
P = [ f/aspect    0         0                        0                    ]
    [ 0           f         0                        0                    ]
    [ 0           0    (far+near)/(near-far)    (2*far*near)/(near-far)  ]
    [ 0           0        -1                        0                    ]

In code:
float f = 1.0f / tanf(fov * 0.5f);
P.m[0][0] = f / aspect;
P.m[1][1] = f;
P.m[2][2] = (far + near) / (near - far);
P.m[3][2] = (2.0f * far * near) / (near - far);
P.m[2][3] = -1.0f;
// All other elements = 0
```

- [ ] **Key property**: Row 3 is [0, 0, -1, 0], so `w_out = -z_in`
- [ ] **After perspective divide**: `(x/w, y/w, z/w)` where w = -z
- [ ] **Result**: Objects farther away (larger z) have larger w, so x/w and y/w become smaller
- [ ] **Example**:
```
Near point: (10, 10, 5) → after projection: w=5  → divide: (2, 2, ...)   [Large on screen]
Far point:  (10, 10, 50) → after projection: w=50 → divide: (0.2, 0.2, ...) [Small on screen]
```

#### Orthographic Projection Matrix
- [ ] **Purpose**: Parallel projection - no perspective, objects same size regardless of depth
- [ ] **Parameters**:
  - `left (l)`, `right (r)`: Horizontal bounds
  - `bottom (b)`, `top (t)`: Vertical bounds
  - `near (n)`, `far (f)`: Depth bounds

- [ ] **Matrix (4×4)**:
```
O = [ 2/(r-l)      0           0         -(r+l)/(r-l) ]
    [ 0          2/(t-b)       0         -(t+b)/(t-b) ]
    [ 0            0        -2/(f-n)     -(f+n)/(f-n) ]
    [ 0            0           0              1       ]

In code:
O.m[0][0] = 2.0f / (right - left);
O.m[1][1] = 2.0f / (top - bottom);
O.m[2][2] = -2.0f / (far - near);
O.m[3][0] = -(right + left) / (right - left);
O.m[3][1] = -(top + bottom) / (top - bottom);
O.m[3][2] = -(far + near) / (far - near);
O.m[3][3] = 1.0f;
// All other elements = 0
```

- [ ] **Example**: Screen from -10 to +10 in X/Y, 0 to 100 in Z
```
l=-10, r=10, b=-10, t=10, n=0, f=100

O = [ 2/20    0      0      0   ]   [ 0.1   0     0      0   ]
    [ 0      2/20    0      0   ] = [ 0     0.1   0      0   ]
    [ 0       0    -2/100  -1   ]   [ 0     0    -0.02  -1   ]
    [ 0       0      0      1   ]   [ 0     0     0      1   ]
```

- [ ] **Usage**: CAD software, 2D games, UI rendering, technical drawings

#### Combined Model Matrix
- [ ] **Full transformation from local to world space**:
```
ModelMatrix = Translation × RotationZ × RotationY × RotationX × Scale

M = T × Rz × Ry × Rx × S

// In code (applying right-to-left):
mat4 model = mat4_identity();
model = mat4_mul(scale_matrix, model);        // Apply scale first
model = mat4_mul(rotation_x_matrix, model);   // Then rotate X
model = mat4_mul(rotation_y_matrix, model);   // Then rotate Y
model = mat4_mul(rotation_z_matrix, model);   // Then rotate Z
model = mat4_mul(translation_matrix, model);  // Finally translate
```

#### Full MVP Matrix
- [ ] **Complete transformation pipeline**:
```
MVP = Projection × View × Model

// Transforms vertex from local space all the way to clip space
vec4 clip_position = MVP × local_vertex;

// Or step by step:
vec4 world_pos = Model × local_vertex;
vec4 view_pos = View × world_pos;
vec4 clip_pos = Projection × view_pos;
```


## Vectors & Operations

### 6. Vector Operations

#### Dot Product (Scalar Product)
- [ ] **Formula**: `A · B = Ax*Bx + Ay*By + Az*Bz`
- [ ] **Alternative**: `A · B = |A| * |B| * cos(θ)` where θ is angle between vectors
- [ ] **When normalized** (length=1): Result is just `cos(θ)`, ranges from -1 to +1
  ```
  Result = +1.0  → Vectors point exactly same direction (0°)
  Result = +0.5  → Vectors at 60° angle
  Result =  0.0  → Vectors perpendicular (90°)
  Result = -0.5  → Vectors at 120° angle
  Result = -1.0  → Vectors point opposite directions (180°)
  ```
- [ ] **Uses**: Check if facing same/opposite direction, calculate angles, lighting

#### Cross Product (Vector Product)
- [ ] **Formula**:
```
A × B = (Ay*Bz - Az*By,
         Az*Bx - Ax*Bz,
         Ax*By - Ay*Bx)
```
- [ ] **Result**: Vector perpendicular to both A and B
- [ ] **Magnitude**: `|A × B| = |A| * |B| * sin(θ)`
- [ ] **Order matters!**: `A × B = -(B × A)` (opposite direction)
- [ ] **Right-hand rule**:
  - Point fingers along A
  - Curl toward B
  - Thumb points in direction of A × B
- [ ] **Uses**: Calculate surface normals, determine winding order

#### Vector Length/Magnitude
- [ ] **Formula**: `|V| = sqrt(Vx² + Vy² + Vz²)`

#### Vector Normalization
- [ ] **Formula**: `V_normalized = V / |V|`
- [ ] **Result**: Vector with same direction, but length = 1
- [ ] **Use**: Before dot product calculations to get cos(θ) directly


### 7. Normals

- [ ] **Definition**: Normal = perpendicular vector to a surface (unit length)
- [ ] **Face normal formula**:
  ```
  edge1 = vertex_b - vertex_a
  edge2 = vertex_c - vertex_a
  normal = normalize(cross(edge2, edge1))
  ```
- [ ] **Winding order matters!**:
  - Counter-clockwise (CCW) vertices → normal points outward
  - Clockwise (CW) vertices → normal points inward
  - Swap edge order in cross product to flip normal direction
- [ ] **Uses**:
  - Lighting calculations (angle between light and surface)
  - Backface culling (is face pointing toward camera?)
  - Collision detection (surface orientation)


## Backface Culling

### 8. Backface Culling Concept

- [ ] **Goal**: Only render faces pointing toward the camera (skip back faces)
- [ ] **Performance**: Saves ~50% of rendering work for closed meshes (box, sphere)
- [ ] **Based on**: Dot product between face normal and view direction
- [ ] **Visual**: Imagine a cube - you only see 3 faces at most, other 3 faces away

### 9. Backface Culling Implementation

```c
// Step 1: Calculate face normal (in view space)
vec3 edge1 = vertex_b - vertex_a;
vec3 edge2 = vertex_c - vertex_a;
vec3 normal = normalize(cross(edge2, edge1));

// Step 2: Get direction from camera to face
vec3 camera_to_face = vertex_a - camera_position;
// In VIEW SPACE: camera_position = (0, 0, 0)
// So: camera_to_face = vertex_a - (0,0,0) = vertex_a

// Step 3: Check if face is visible
float dot_result = dot(normal, camera_to_face);
if (dot_result < 0) {
    // Face is visible (normal and view direction point away from each other)
    render_face();
} else {
    // Face pointing away, cull it
    skip_face();
}
```

- [ ] **Key insight**: If normal and camera_to_face point opposite directions (dot < 0), face is visible
- [ ] **CRITICAL**: All vectors must be in the SAME coordinate space!


### 10. Common Backface Culling Bug

**The Bug We Fixed:**
```c
// BUG: Mixing coordinate spaces
vec3 view_space_vertex = ...;  // In view space
vec3 view_space_normal = ...;  // In view space
vec3 world_camera = (10, 5, 20); // In WORLD space

vec3 camera_to_face = view_space_vertex - world_camera; // WRONG!
// This is like subtracting meters from feet!
```

**The Fix:**
```c
// CORRECT: Same coordinate space
vec3 view_space_vertex = ...;  // In view space
vec3 view_space_normal = ...;  // In view space
vec3 view_space_camera = (0, 0, 0); // Camera in VIEW space

vec3 camera_to_face = view_space_vertex - view_space_camera; // RIGHT!
// = view_space_vertex - (0,0,0) = view_space_vertex
```

- [ ] **Rule**: In view space, camera position is ALWAYS (0, 0, 0)
- [ ] **Verification**: Check which space your vertices are in before culling


## Projection & Perspective

### 11. Projection Types

#### Perspective Projection
- [ ] **Characteristic**: Objects farther away appear smaller (realistic)
- [ ] **How it works**: Uses projection matrix that divides by Z (depth)
- [ ] **Field of view (FOV)**: Wider FOV = more "fisheye" effect
- [ ] **Uses**: Games, realistic rendering, 3D visualization

#### Orthographic Projection
- [ ] **Characteristic**: Objects same size regardless of distance
- [ ] **How it works**: Parallel projection, no depth scaling
- [ ] **Uses**: CAD, technical drawings, 2D games, UI elements
- [ ] **Example**: Engineering blueprints, architectural plans


### 12. Perspective Division

- [ ] **When**: After projection matrix, before screen mapping
- [ ] **Formula**:
  ```
  x_ndc = x_clip / w_clip
  y_ndc = y_clip / w_clip
  z_ndc = z_clip / w_clip
  ```
- [ ] **What W represents**: Distance from camera (roughly -Z in view space)
- [ ] **Effect**:
  ```
  Close object: w = 2  → (10, 10, 5) / 2 = (5, 5, 2.5)  → Large on screen
  Far object:   w = 10 → (10, 10, 5) / 10 = (1, 1, 0.5) → Small on screen
  ```
- [ ] **This creates perspective!** Things farther (larger W) become smaller


## Rendering Pipeline

### 13. Typical Rendering Order

```
1. Vertex Transformation
   ├─ Model Matrix (local → world)
   ├─ View Matrix (world → view)
   └─ [Camera is now at origin]

2. Backface Culling (in view space)
   └─ Skip triangles facing away

3. Projection
   └─ Projection Matrix (view → clip)

4. Clipping
   └─ Remove triangles outside view frustum

5. Perspective Divide
   └─ (x/w, y/w, z/w) → NDC space

6. Viewport Transform
   └─ NDC (-1 to +1) → Screen pixels (0 to width/height)

7. Rasterization
   ├─ Determine which pixels triangle covers
   ├─ Interpolate attributes (UV, colors, depth)
   └─ Depth test (Z-buffer)

8. Shading/Texturing
   └─ Color each pixel
```

**Detailed Steps:**

1. [ ] **Transform vertices to view space**: Apply Model then View matrix
2. [ ] **Perform backface culling** (in view space): Skip back-facing triangles
3. [ ] **Transform to clip space**: Apply projection matrix
4. [ ] **Clip triangles to view frustum**: Cut triangles at screen boundaries
5. [ ] **Perspective divide** (to NDC): Divide x,y,z by w component
6. [ ] **Viewport transform** (to screen pixels): Map NDC (-1,+1) to (0, screen_width/height)
7. [ ] **Rasterize and shade**: Fill pixels, apply textures/lighting


### 14. Coordinate System Handedness

#### Left-Handed System
- [ ] **Rule**: Z increases into screen (+Z forward, away from camera)
- [ ] **Thumb = X, Index = Y, Middle = Z** (left hand)
- [ ] **Used by**: DirectX, Unity (left-handed by default)
- [ ] **Cross product**: A × B points "up" relative to right-hand rule, but used with left hand

#### Right-Handed System
- [ ] **Rule**: Z increases out of screen (-Z forward, toward camera)
- [ ] **Thumb = X, Index = Y, Middle = Z** (right hand)
- [ ] **Used by**: OpenGL, Blender, many CAD tools
- [ ] **Cross product**: Standard right-hand rule

#### Why it matters
- [ ] **Cross product winding**: Affects which way normals point
- [ ] **Matrix formulas**: Some signs flip between systems
- [ ] **View matrix**: Forward vector sign differs
- [ ] **Your engine**: Check which system you're using! (Affects backface culling)


## UV Texture Mapping

### 15. UV Coordinates Explained

- [ ] **U = horizontal** axis in texture (0.0 = left edge, 1.0 = right edge)
- [ ] **V = vertical** axis in texture (0.0 = bottom edge, 1.0 = top edge)
- [ ] **Per-vertex UVs**: Each vertex has a UV coordinate
- [ ] **Interpolation**: UV values interpolated across triangle during rasterization

#### Full Texture Mapping (0-1 range)
```
Triangle UVs:
  v0: (0.0, 0.0) ──────────┐
  v1: (1.0, 0.0)           │ Full texture
  v2: (0.5, 1.0)           │ displayed on face
                 ──────────┘
```

#### Partial Texture Mapping (UV Atlas)
```
Triangle UVs:
  v0: (0.25, 0.5)  ──────┐
  v1: (0.5, 0.5)         │ Only 25% of texture
  v2: (0.375, 0.75)      │ shown (appears "zoomed")
                   ──────┘
```

### 16. UV Atlas vs Simple Mapping

#### Simple Mapping
- [ ] **UVs**: All faces use (0,0) to (1,1) range
- [ ] **Result**: Same texture on every face
- [ ] **Texture needed**: One square image
- [ ] **Use case**: Repeating patterns (bricks, tiles)

#### UV Atlas (Unwrap)
- [ ] **UVs**: Each face mapped to different texture region
- [ ] **Result**: Different texture on each face
- [ ] **Texture needed**: Atlas image with all face designs
- [ ] **Use case**: Unique designs per face (characters, complex objects)
- [ ] **Example**: Dice with different numbers on each face


## Common Mistakes to Avoid

### 17. Checklist of Common Errors

- [ ] **Mixing coordinate spaces in calculations**
  - Always check: Are all vectors in the same space?

- [ ] **Wrong matrix multiplication order**
  - Remember: RIGHT to LEFT execution
  - `T × R × S` means Scale first, Rotate second, Translate last

- [ ] **Forgetting to normalize vectors for dot products**
  - Dot of unnormalized vectors: `A·B = |A|*|B|*cos(θ)` (magnitude affects result)
  - Dot of normalized vectors: `A·B = cos(θ)` (only angle matters)

- [ ] **Wrong cross product winding order**
  - `A × B = -(B × A)` (order flips direction!)
  - Check your coordinate system handedness

- [ ] **Applying transformations in wrong order**
  - Correct: Scale → Rotate → Translate (SRT)
  - Wrong: Translate → Rotate → Scale (object rotates around wrong point)

- [ ] **Using world space camera position in view space calculations**
  - View space camera is ALWAYS (0, 0, 0)

- [ ] **UV coordinates outside 0-1 without texture wrapping**
  - Either clamp UVs or handle wrapping/repeating

- [ ] **Integer division when calculating slopes**
  - Use float: `(float)(b.x - a.x) / (float)(b.y - a.y)`
  - Not: `(b.x - a.x) / (b.y - a.y)` ← truncates to 0 for small deltas!


## Quick Self-Test Questions

- [ ] **Q**: Where is the camera in view space?
      **A**: (0, 0, 0) - always at the origin

- [ ] **Q**: What does dot product measure?
      **A**: Alignment/similarity of vectors. Normalized: cos(angle between them)

- [ ] **Q**: What does cross product create?
      **A**: A vector perpendicular to both input vectors

- [ ] **Q**: Why can't I mix coordinate spaces?
      **A**: Each space has different origin and orientation - like mixing meters and feet

- [ ] **Q**: What order are transformations applied?
      **A**: Scale, then Rotation, then Translation (SRT) - read matrices right-to-left

- [ ] **Q**: When should I do backface culling?
      **A**: After view transformation (in view space), before projection

- [ ] **Q**: If `dot(normal, camera_to_face) = -0.8`, is face visible?
      **A**: Yes! Negative dot means vectors point away from each other (face toward camera)

- [ ] **Q**: What does perspective division do?
      **A**: Divides x,y,z by w to create depth effect (farther objects appear smaller)

- [ ] **Q**: Why does `T × R × S` apply Scale first?
      **A**: Matrix multiplication is right-to-left. S is rightmost, applied first to vertex

- [ ] **Q**: UV coordinates (0.5, 0.5) sample which part of texture?
      **A**: Center of the texture (halfway in U and V)


## Practical Examples

### Example 1: Transforming a Vertex

```c
// Vertex in local space
vec3 v_local = (1, 0, 0);

// Transformations
mat4 scale = make_scale(2, 2, 2);           // Double size
mat4 rotate_y = make_rotation_y(PI/4);      // Rotate 45° around Y
mat4 translate = make_translation(10, 0, 0); // Move right 10 units

// Combined model matrix (remember: right to left!)
mat4 model = translate × rotate_y × scale;

// Apply transformation
vec3 v_world = model × v_local;
// Result: (1,0,0) → scale(2,0,0) → rotate_y(1.41,0,-1.41) → translate(11.41,0,-1.41)
```

### Example 2: Backface Culling Check

```c
// In view space (after view matrix applied)
vec3 v0 = (2, 1, 5);
vec3 v1 = (4, 1, 5);
vec3 v2 = (3, 3, 5);

// Calculate normal
vec3 edge1 = v1 - v0;  // (2, 0, 0)
vec3 edge2 = v2 - v0;  // (1, 2, 0)
vec3 normal = normalize(cross(edge2, edge1));  // (0, 0, 1) pointing toward camera

// Camera in view space
vec3 camera = (0, 0, 0);

// Direction from camera to face
vec3 camera_to_face = v0 - camera;  // (2, 1, 5)

// Check visibility
float d = dot(normal, camera_to_face);  // 0*2 + 0*1 + 1*5 = 5
if (d < 0) {
    // Face visible (facing camera)
    render();
} else {
    // Face culled (5 > 0, so face pointing away)
    skip();
}
```


---

## Review Frequency

- **Daily (5-10 min)**:
  - Sections 1-2 (Coordinate spaces)
  - Section 6 (Vector operations)
  - Sections 9-10 (Backface culling)

- **Weekly (20-30 min)**:
  - Full checklist
  - Practice self-test questions
  - Review any sections you got wrong

- **Before coding**:
  - Review sections relevant to current task
  - If working on transformations → Section 5 (Matrix formulas)
  - If working on rendering pipeline → Section 13
  - If debugging → Section 17 (Common mistakes)

- **When stuck on a bug**:
  - Check Section 17 first
  - Verify coordinate space consistency (Section 2)
  - Check transformation order (Section 4)


---

**Last Updated**: 2025-10-30
**Version**: 2.0 - Enhanced with formulas and detailed context
