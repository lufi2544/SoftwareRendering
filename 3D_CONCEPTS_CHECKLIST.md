# 3D Graphics Concepts - Daily Review Checklist

## Coordinate Spaces & Transformations

### 1. The Four Main Coordinate Spaces
- [ ] **Model/Local Space** - Vertices defined relative to mesh origin (0,0,0)
- [ ] **World Space** - After applying model matrix (scale, rotation, translation)
- [ ] **View/Camera Space** - After applying view matrix (camera at origin)
- [ ] **Clip/Screen Space** - After applying projection matrix

### 2. Key Rule: NEVER Mix Coordinate Spaces
- [ ] When doing calculations (dot products, distances, directions), ALL vectors must be in the SAME space
- [ ] In view space, camera is ALWAYS at (0, 0, 0)
- [ ] Each transformation matrix converts from one space to the next

### 3. Transformation Pipeline Order
```
Vertex → [Model Matrix] → World Space → [View Matrix] → View Space → [Projection Matrix] → Clip Space
```
- [ ] Model Matrix = Scale × Rotation × Translation (SRT order matters!)
- [ ] View Matrix = Moves world so camera is at origin
- [ ] Projection Matrix = Applies perspective/orthographic projection


## Matrices & Transformations

### 4. Matrix Multiplication Order Matters
- [ ] Matrices are applied RIGHT to LEFT: `C = B × A` means "apply A first, then B"
- [ ] For transformations: `World = Translation × Rotation × Scale`
- [ ] For vectors: `v' = Matrix × v`

### 5. Common Matrix Types
- [ ] **Translation** - Moves objects in space
- [ ] **Rotation** - Rotates around X, Y, or Z axis
- [ ] **Scale** - Makes objects bigger/smaller
- [ ] **View** - Transforms world to camera view
- [ ] **Projection** - Applies perspective or orthographic projection


## Vectors & Operations

### 6. Vector Operations
- [ ] **Dot Product** - Measures how aligned two vectors are (-1 to +1 when normalized)
  - Positive = pointing same direction
  - Zero = perpendicular
  - Negative = pointing opposite directions
- [ ] **Cross Product** - Creates perpendicular vector (normal to surface)
  - Order matters! `A × B != B × A` (opposite direction)
  - Right-hand rule determines direction

### 7. Normals
- [ ] Normal = perpendicular vector to a surface
- [ ] Face normal = `cross(edge1, edge2)` normalized
- [ ] Winding order (CW vs CCW) affects normal direction
- [ ] Use for: lighting, backface culling, collision detection


## Backface Culling

### 8. Backface Culling Concept
- [ ] Only render faces pointing toward the camera
- [ ] Saves ~50% of rendering work (for closed meshes)
- [ ] Based on dot product between face normal and view direction

### 9. Backface Culling Implementation
- [ ] Calculate face normal from vertices
- [ ] Get direction vector from camera to face
- [ ] `dot(normal, camera_to_face) < 0` means face is visible
- [ ] **CRITICAL**: All vectors must be in the SAME coordinate space!

### 10. Common Backface Culling Bug
- [ ] BUG: Using world space camera position with view space vertices
- [ ] FIX: In view space, camera position is (0, 0, 0)
- [ ] VERIFY: Check which space your vertices are in before culling


## Projection & Perspective

### 11. Projection Types
- [ ] **Perspective** - Objects farther away appear smaller (realistic)
- [ ] **Orthographic** - Objects same size regardless of distance (technical drawings)
- [ ] Projection matrix converts 3D coordinates to 2D screen space

### 12. Perspective Division
- [ ] After projection, divide by W component: `(x/w, y/w, z/w)`
- [ ] This creates the "depth" effect
- [ ] Closer objects have smaller W, appear larger after division


## Rendering Pipeline

### 13. Typical Rendering Order
1. [ ] Transform vertices to view space
2. [ ] Perform backface culling (in view space)
3. [ ] Transform to clip space (projection)
4. [ ] Clip triangles to view frustum
5. [ ] Perspective divide (to NDC - Normalized Device Coordinates)
6. [ ] Viewport transform (to screen pixels)
7. [ ] Rasterize and shade

### 14. Coordinate System Handedness
- [ ] **Left-handed**: Z increases into screen (+Z forward)
- [ ] **Right-handed**: Z increases out of screen (-Z forward)
- [ ] Know which your engine uses! (Affects cross product winding)


## Common Mistakes to Avoid

### 15. Checklist of Common Errors
- [ ] Mixing coordinate spaces in calculations
- [ ] Wrong matrix multiplication order
- [ ] Forgetting to normalize vectors for dot products
- [ ] Wrong cross product winding order
- [ ] Applying transformations in wrong order (should be SRT)
- [ ] Using world space camera position in view space calculations


## Quick Self-Test Questions

- [ ] Q: Where is the camera in view space? A: (0, 0, 0)
- [ ] Q: What does dot product measure? A: Alignment of vectors
- [ ] Q: What does cross product create? A: Perpendicular vector
- [ ] Q: Why can't I mix coordinate spaces? A: Vectors represent different frames of reference
- [ ] Q: What order are transformations applied? A: Scale, then Rotation, then Translation
- [ ] Q: When should I do backface culling? A: After view transformation, before projection


---

## Review Frequency
- **Daily**: Sections 1-2, 6, 9-10
- **Weekly**: Full checklist
- **Before coding**: Sections relevant to current task

Last Updated: 2025-10-30
