import { ReactNode } from "react";
import Sidebar from "../components/sidebar";
import Header from "../components/header";

interface Props {
    children: ReactNode;
}

export default function MainLayout({ children }: Props) {
    return (
        <div className="flex min-h-screen w-screen bg-gray-100">
            <Sidebar />
            <div className = "flex flex-col flex-1">
                <Header />
                <main className = "p-8 overflow-y-auto">
                    {children}
                </main>
            </div>
        </div>
    )
}