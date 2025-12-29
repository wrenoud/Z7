# Z7

This is a work-in-progress to explore the possibility of neighbor traversal in the Z7 domain. Z7 is an indexing system
for aperature 7 hexagonal discrete global grid systems, as coined in [Kmoch et al. (2025)][kmoch2025]. It is based on
the Generalized Balanced Ternary (GBT) numeral system described in [Lucas, Gibson (1982)][lucas1982],
[van Roessel (1988)][vanRoessel1988], and [Wikipedia (2025)][wikipedia2025].

### Rotation Pattern

Lucas and Gibson (1982) gave examples of GBT with exclusively counter-clockwise (CCW) rotation. The disadvantage of this
approach is that the orientation of the hexagons at the different hierarchy levels quickly diverge from each other.
[White et al. (1992)][white1992] proposed an alternating rotation direction (CW, CCW, CW, ...) to maintain an alignment
of hexagon
orientations across hierarchy levels. Kmoch et al. (2025) adopted this alternating rotation pattern for Z7 assigning CW
to odd resolutions and CCW to even resolutions.

For reference purposes a rendering of three hierarchies of alternating GBT rotation is included (CW, CCW, CW).

<img src="./images/grid.svg" width="200" alt="3 hierarchies in Z7's alternating GBT pattern">

### GBT Addition

| Counter-clockwise (CCW)                                                                                        | Clockwise (CW)                                                                                               |
|----------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| <img src="./images/neighbors-neighbors-ccw.svg" width="300" alt="Single digit GBT addition with CCW rotation"> | <img src="./images/neighbors-neighbors-cw.svg" width="300" alt="Single digit GBT addition with CW rotation"> |

## References

1. Kmoch, A., Sahr, K., Chan, W. T., & Uuemaa, E. (2025).
   [IGEO7: A new hierarchically indexed hexagonal equal-area discrete global grid system][kmoch2025]. _AGILE: GIScience
   Series, 6_(32). https://doi.org/10.5194/agile-giss-6-32-2025
2. Lucas, D., & Gibson, L. (1982). [_Automated Analysis of imagery_][lucas1982]. Air Force Office of Scientific
   Research. https://apps.dtic.mil/sti/tr/pdf/ADA125710.pdf. (No. AFOSRTR830055).
3. Sahr, K. (2011). [Hexagonal discrete global grid systems for geospatial computing][sahr2011]. _Archives of
   Photogrammetry Cartography and Remote Sensing, 22_, 363–376.
4. van Roessel, J. W. (1988).
   [Conversion of Cartesian coordinates from and to Generalized Balanced Ternary addresses][vanRoessel1988].
   _Photogrammetric Engineering & Remote Sensing, 54_(11), 1565–1570.
5. White, D., Kimerling, J. A., & Overton, S. W. (1992).
   [Cartographic and Geometric Components of a Global Sampling Design for Environmental Monitoring][white1992].
   _Cartography and Geographic Information Systems_, 19(1), 5-22. https://doi.org/10.1559/152304092783786636
6. Wikipedia Contributors. (2025, December 16). [_Generalized balanced ternary_][wikipedia2025]. Wikipedia; Wikimedia
   Foundation.

---

[wikipedia2025]: https://en.wikipedia.org/wiki/Generalized_balanced_ternary

[kmoch2025]: https://agile-giss.copernicus.org/articles/6/32/2025/agile-giss-6-32-2025.pdf

[vanRoessel1988]: https://web.archive.org/web/20250523045258/https://www.asprs.org/wp-content/uploads/pers/1988journal/nov/1988_nov_1565-1570.pdf

[white1992]: https://www.tandfonline.com/doi/pdf/10.1559/152304092783786636?casa_token=lyaVPE0vjcQAAAAA:GL0QWw01pvr2AQWJgmNcjHH_rCp3VLv5jayMi7O9Kb7jC_l2KFt2b98GvnRcOhi4qSXV-FXXxUYAVg

[sahr2011]: https://www.researchgate.net/publication/266415994_Hexagonal_discrete_global_grid_systems_for_geospatial_computing

[lucas1982]: https://apps.dtic.mil/sti/tr/pdf/ADA125710.pdf