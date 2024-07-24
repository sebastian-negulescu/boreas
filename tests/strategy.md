# Testing Strategy

## Questions
What should we test?
How should we test?

## Categories to Test
- [x] Camera functionality
- [x] Casting rays into world
    - [x] Make sure geometry here is correct with FOV 
- Intersecting rays with spheres
- Diffuse and luminescent materials

## Strategies (Tests) for Each Component

### Camera
- Given the look from, look at, and up points/vectors, give back the correct u, v, and w rays
    - Rays will be the camera's coordinate frame
- Try both axis-aligned and arbitraty look from, look at, and up points
- Try error cases: look from and look at points are the same

### Ray Casting
- Check with known geometry that rays are properly cast into world

### Intersection with Spheres

