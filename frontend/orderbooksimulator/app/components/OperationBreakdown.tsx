"use client";

import React from "react";
import { Card, CardContent, Typography, Box, Grid } from "@mui/material";
import {
  PieChart,
  Pie,
  Cell,
  BarChart,
  Bar,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  Legend,
  ResponsiveContainer,
} from "recharts";
import { Metrics } from "@/app/types/metrics";
import { formatNumber } from "@/app/utils/formatters";

interface OperationBreakdownProps {
  metrics: Metrics;
  color: string;
}

const COLORS = ["#0088FE", "#00C49F", "#FFBB28"];

export default function OperationBreakdown({
  metrics,
  color,
}: OperationBreakdownProps) {
  const pieData = [
    { name: "Add", value: metrics.addCount },
    { name: "Cancel", value: metrics.cancelCount },
    { name: "Modify", value: metrics.modifyCount },
  ];

  const barData = [
    { name: "Add", count: metrics.addCount },
    { name: "Cancel", count: metrics.cancelCount },
    { name: "Modify", count: metrics.modifyCount },
  ];

  return (
    <Box>
      <Typography
        variant="h6"
        component="h3"
        gutterBottom
        sx={{ color, fontWeight: 600, mb: 2 }}
      >
        Operation Breakdown
      </Typography>
      <Grid container spacing={2}>
        <Grid item xs={12} md={6}>
          <Card>
            <CardContent>
              <Typography variant="subtitle1" gutterBottom>
                Operation Distribution
              </Typography>
              <ResponsiveContainer width="100%" height={300}>
                <PieChart>
                  <Pie
                    data={pieData}
                    cx="50%"
                    cy="50%"
                    labelLine={false}
                    label={({ name, percent }) =>
                      `${name}: ${(percent * 100).toFixed(0)}%`
                    }
                    outerRadius={80}
                    fill="#8884d8"
                    dataKey="value"
                  >
                    {pieData.map((entry, index) => (
                      <Cell
                        key={`cell-${index}`}
                        fill={COLORS[index % COLORS.length]}
                      />
                    ))}
                  </Pie>
                  <Tooltip formatter={(value: number) => formatNumber(value)} />
                  <Legend />
                </PieChart>
              </ResponsiveContainer>
            </CardContent>
          </Card>
        </Grid>

        <Grid item xs={12} md={6}>
          <Card>
            <CardContent>
              <Typography variant="subtitle1" gutterBottom>
                Operation Counts
              </Typography>
              <ResponsiveContainer width="100%" height={300}>
                <BarChart data={barData}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="name" />
                  <YAxis
                    label={{
                      value: "Count",
                      angle: -90,
                      position: "left",
                      offset: 0,
                    }}
                    tickFormatter={(value: number) => formatNumber(value)}
                    width={80}
                  />
                  <Tooltip formatter={(value: number) => formatNumber(value)} />
                  <Legend />
                  <Bar dataKey="count" fill={color} />
                </BarChart>
              </ResponsiveContainer>
            </CardContent>
          </Card>
        </Grid>
      </Grid>
    </Box>
  );
}
