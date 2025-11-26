import {NavLink} from "react-router-dom";

export default function Sidebar() {
    const linkBase = "flex items-center gap-3 px-4 py-3 rounded-lg hover:bg-blue-600";
    const active = "bg-blue-600 text-white hover:bg-blue-600";

    return (
        <aside className="w-64 p-6 border-r bg-white">
            <h1 className="font-bold text-xl mb-8">Posture Detector</h1>

            <nav className="flex flex-col gap-3">
                <NavLink to="/" className={({isActive}) => isActive ? `${linkBase} ${active}` : linkBase}>
                    Dashboard
                </NavLink>
                <NavLink to="/historial" className={({isActive}) => isActive ? `${linkBase} ${active}` : linkBase}>
                    Historial
                </NavLink>
                <NavLink to="/configuracion" className={({isActive}) => isActive ? `${linkBase} ${active}` : linkBase}>
                    Configuración
                </NavLink>
                <NavLink to="/consejos" className={({isActive}) => isActive ? `${linkBase} ${active}` : linkBase}>
                    Consejos
                </NavLink>
            </nav>
        </aside>
    );
}