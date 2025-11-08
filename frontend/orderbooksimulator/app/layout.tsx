import type { Metadata } from "next";
import ThemeProviderWrapper from "./components/ThemeProviderWrapper";

export const metadata: Metadata = {
  title: "Order Book Simulator Dashboard",
  description: "Performance metrics dashboard for Order Book Simulator",
};

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body>
        <ThemeProviderWrapper>{children}</ThemeProviderWrapper>
      </body>
    </html>
  );
}

