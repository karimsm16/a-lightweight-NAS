/******************************************************************************
 * script.js
 * ESP32 NAS
 ******************************************************************************/

/******************************************************************************
 * Global Variables
 ******************************************************************************/

let currentPath = "/";
let selectedFile = null;

/******************************************************************************
 * HTML Elements
 ******************************************************************************/

// Sidebar
const navFiles = document.getElementById("nav-files");
const navNetwork = document.getElementById("nav-network");
const navSettings = document.getElementById("nav-settings");

// Pages
const pageFiles = document.getElementById("page-files");
const pageNetwork = document.getElementById("page-network");
const pageSettings = document.getElementById("page-settings");

// Explorer
const fileList = document.getElementById("file-list");
const breadcrumb = document.getElementById("breadcrumb");

// Toolbar
const btnRefresh = document.getElementById("btn-refresh");
const btnBack = document.getElementById("btn-back");
const btnUpload = document.getElementById("btn-upload");
const btnFolder = document.getElementById("btn-folder");
const btnRename = document.getElementById("btn-rename");
const btnDelete = document.getElementById("btn-delete");
const btnDownload = document.getElementById("btn-download");

// Search
const searchBox = document.querySelector(".search-box input");

/******************************************************************************
 * Navigation
 ******************************************************************************/

function showPage(page, button)
{
    pageFiles.classList.remove("active");
    pageNetwork.classList.remove("active");
    pageSettings.classList.remove("active");

    navFiles.classList.remove("active");
    navNetwork.classList.remove("active");
    navSettings.classList.remove("active");

    page.classList.add("active");
    button.classList.add("active");
}

/******************************************************************************
 * Sidebar Events
 ******************************************************************************/

navFiles.addEventListener("click", () =>
{
    showPage(pageFiles, navFiles);
    loadFiles(currentPath);
});

navNetwork.addEventListener("click", () =>
{
    showPage(pageNetwork, navNetwork);
    loadNetwork();
});

navSettings.addEventListener("click", () =>
{
    showPage(pageSettings, navSettings);
});

/******************************************************************************
 * Network
 ******************************************************************************/

async function loadNetwork()
{
    try
    {
        const response = await fetch("/api/network");
        const data = await response.json();

        document.getElementById("network-status").textContent = data.status;
        document.getElementById("network-ssid").textContent = data.ssid;
        document.getElementById("network-ip").textContent = data.ip;
        document.getElementById("network-signal").textContent = data.signal + " dBm";
        document.getElementById("network-mac").textContent = data.mac;
        document.getElementById("network-gateway").textContent = data.gateway;
    }
    catch(error)
    {
        console.error(error);
    }
}

/******************************************************************************
 * File Explorer
 ******************************************************************************/

async function loadFiles(path = "/")
{
    currentPath = path;

    try
    {
        const response =
            await fetch("/api/files?path=" + encodeURIComponent(path));

        const files = await response.json();

        fileList.innerHTML = "";

        files.forEach(file =>
        {
            const row = document.createElement("tr");

            row.dataset.path =
                (currentPath === "/" ? "" : currentPath) +
                "/" +
                file.name;

            row.innerHTML = `
                <td>${file.type === "folder" ? "📁" : "📄"}</td>
                <td>${file.name}</td>
                <td>${file.type === "folder" ? "-" : formatSize(file.size)}</td>
                <td>${file.type}</td>
            `;

            row.addEventListener("click", () =>
            {
                document.querySelectorAll("#file-list tr")
                    .forEach(r => r.classList.remove("selected"));

                row.classList.add("selected");

                selectedFile =
                {
                    name: file.name,
                    type: file.type,
                    path: row.dataset.path
                };
            });

            row.addEventListener("dblclick", () =>
            {
                if(file.type !== "folder")
                    return;

                const next =
                    currentPath === "/"
                    ? "/" + file.name
                    : currentPath + "/" + file.name;

                loadFiles(next);
            });

            fileList.appendChild(row);
        });

        updateBreadcrumb(currentPath);
    }
    catch(error)
    {
        console.error("Failed to load files:", error);
    }
}

/******************************************************************************
 * Breadcrumb
 ******************************************************************************/

function updateBreadcrumb(path)
{
    breadcrumb.innerHTML = "";

    const home = document.createElement("a");
    home.textContent = "Home";
    home.onclick = () => loadFiles("/");

    breadcrumb.appendChild(home);

    if(path === "/")
        return;

    let current = "";

    path.split("/")
        .filter(x => x)
        .forEach(folder =>
        {
            breadcrumb.append(" > ");

            current += "/" + folder;

            const link = document.createElement("a");
            link.textContent = folder;
            link.onclick = () => loadFiles(current);

            breadcrumb.appendChild(link);
        });
}

/******************************************************************************
 * Toolbar
 ******************************************************************************/

btnRefresh.addEventListener("click", () =>
{
    loadFiles(currentPath);
});

btnBack.addEventListener("click", () =>
{
    if(currentPath === "/")
        return;

    const parts = currentPath.split("/");
    parts.pop();

    currentPath = parts.join("/");

    if(currentPath === "")
        currentPath = "/";

    loadFiles(currentPath);
});

/******************************************************************************
 * Helper
 ******************************************************************************/

function formatSize(bytes)
{
    if(bytes < 1024)
        return bytes + " B";

    if(bytes < 1024 * 1024)
        return (bytes / 1024).toFixed(1) + " KB";

    if(bytes < 1024 * 1024 * 1024)
        return (bytes / 1024 / 1024).toFixed(1) + " MB";

    return (bytes / 1024 / 1024 / 1024).toFixed(1) + " GB";
}
/******************************************************************************
 * Storage
 ******************************************************************************/
async function loadStorage()
{
    try
    {
        console.log("loadStorage called");

        const response = await fetch("/api/storage");
        const data = await response.json();

        console.log(data);

        const progress = document.querySelector(".progress-fill");
        const text = document.getElementById("storage-text");
        const footer = document.getElementById("footer-storage");

        console.log(progress);
        console.log(text);
        console.log(footer);

        const percent = (data.used / data.total) * 100;

        if(progress)
            progress.style.width = percent + "%";

        if(text)
            text.textContent =
                formatSize(data.used) + " / " + formatSize(data.total);

        if(footer)
            footer.textContent =
                formatSize(data.used) + " / " + formatSize(data.total);
    }
    catch(error)
    {
        console.error("Storage:", error);
    }
}
/******************************************************************************
 * Download
 ******************************************************************************/

btnDownload.addEventListener("click", () =>
{
    if(!selectedFile)
    {
        alert("Select a file.");
        return;
    }

    if(selectedFile.type === "folder")
    {
        alert("Folders cannot be downloaded.");
        return;
    }

    window.location =
        "/download?file=" +
        encodeURIComponent(selectedFile.path);
});

/******************************************************************************
 * Delete
 ******************************************************************************/

btnDelete.addEventListener("click", async () =>
{
    if(!selectedFile)
    {
        alert("Select a file.");
        return;
    }

    if(!confirm("Delete " + selectedFile.name + "?"))
        return;

    await fetch(
        "/api/delete?file=" +
        encodeURIComponent(selectedFile.path)
    );

    selectedFile = null;

    loadFiles(currentPath);
});

/******************************************************************************
 * Upload
 ******************************************************************************/

const uploadInput = document.createElement("input");
uploadInput.type = "file";
uploadInput.hidden = true;

document.body.appendChild(uploadInput);

btnUpload.addEventListener("click", () =>
{
    uploadInput.click();
});

uploadInput.addEventListener("change", async () =>
{
    if(uploadInput.files.length === 0)
        return;

    const form = new FormData();

    form.append("file", uploadInput.files[0]);

    await fetch("/api/upload",
    {
        method: "POST",
        body: form
    });

    uploadInput.value = "";

    loadFiles(currentPath);
});

/******************************************************************************
 * Rename
 ******************************************************************************/

btnRename.addEventListener("click", async () =>
{
    if(!selectedFile)
    {
        alert("Select a file.");
        return;
    }

    const newName = prompt(
        "New name:",
        selectedFile.name
    );

    if(!newName)
        return;

    const oldPath = selectedFile.path;

    let newPath;

    if(currentPath === "/")
        newPath = "/" + newName;
    else
        newPath = currentPath + "/" + newName;

    await fetch("/api/rename",
    {
        method: "POST",
        headers:
        {
            "Content-Type":"application/x-www-form-urlencoded"
        },
        body:
            "old=" + encodeURIComponent(oldPath) +
            "&new=" + encodeURIComponent(newPath)
    });

    selectedFile = null;

    loadFiles(currentPath);
});

/******************************************************************************
 * Create Folder
 ******************************************************************************/

btnFolder.addEventListener("click", async () =>
{
    const folder = prompt("Folder name:");

    if(!folder)
        return;

    const path =
        currentPath === "/"
        ? "/" + folder
        : currentPath + "/" + folder;

    await fetch("/api/mkdir",
    {
        method:"POST",
        headers:
        {
            "Content-Type":"application/x-www-form-urlencoded"
        },
        body:"path=" + encodeURIComponent(path)
    });

    loadFiles(currentPath);
});

/******************************************************************************
 * Search
 ******************************************************************************/

if(searchBox)
{
    searchBox.addEventListener("keyup", () =>
    {
        const value = searchBox.value.toLowerCase();

        fileList.querySelectorAll("tr").forEach(row =>
        {
            row.style.display =
                row.innerText.toLowerCase().includes(value)
                ? ""
                : "none";
        });
    });
}

/******************************************************************************
 * Auto Refresh
 ******************************************************************************/

setInterval(() =>
{
    loadFiles(currentPath);
    loadStorage();
}, 5000);

/******************************************************************************
 * Startup
 ******************************************************************************/

window.addEventListener("DOMContentLoaded", () =>
{
    showPage(pageFiles, navFiles);

    loadFiles("/");
    loadNetwork();
    loadStorage();
});