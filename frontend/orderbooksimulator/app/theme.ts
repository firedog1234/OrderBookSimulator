"use client";

import { createTheme } from "@mui/material/styles";

// Custom colors for RB Tree (red) and Skip List (blue)
export const RB_TREE_COLOR = "#d32f2f"; // Red
export const SKIP_LIST_COLOR = "#1976d2"; // Blue

export const theme = createTheme({
  palette: {
    primary: {
      main: "#1976d2",
    },
    secondary: {
      main: "#d32f2f",
    },
  },
  typography: {
    h4: {
      fontWeight: 600,
    },
    h5: {
      fontWeight: 600,
    },
    h6: {
      fontWeight: 600,
    },
  },
  components: {
    MuiCard: {
      styleOverrides: {
        root: {
          boxShadow: "0 2px 8px rgba(0,0,0,0.1)",
        },
      },
    },
  },
});

