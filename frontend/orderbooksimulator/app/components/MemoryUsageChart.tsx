"use client";

import React from "react";
import { Card, CardContent, Typography, Box } from "@mui/material";
import {
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
import { formatMemory } from "@/app/utils/formatters";

interface MemoryUsageChartProps {
  rbTree?: Metrics;
  skipList?: Metrics;
}

export default function MemoryUsageChart({
  rbTree,
  skipList,
}: MemoryUsageChartProps) {
  const data = [
    {
      name: "Red-Black Tree",
      memory: rbTree?.memoryUsageBytes || 0,
    },
    {
      name: "Skip List",
      memory: skipList?.memoryUsageBytes || 0,
    },
  ];

  return (
    <Box>
      <Typography variant="h6" component="h3" gutterBottom sx={{ mb: 2 }}>
        Memory Usage Comparison
      </Typography>
      <Card>
        <CardContent>
          <ResponsiveContainer width="100%" height={300}>
            <BarChart data={data}>
              <CartesianGrid strokeDasharray="3 3" />
              <XAxis dataKey="name" />
              <YAxis
                label={{
                  value: "Memory",
                  angle: -90,
                  position: "left",
                  offset: 0,
                }}
                tickFormatter={(value: number) => formatMemory(value)}
                width={90}
              />
              <Tooltip formatter={(value: number) => formatMemory(value)} />
              <Legend />
              <Bar dataKey="memory" fill="#1976d2" />
            </BarChart>
          </ResponsiveContainer>
        </CardContent>
      </Card>
    </Box>
  );
}
